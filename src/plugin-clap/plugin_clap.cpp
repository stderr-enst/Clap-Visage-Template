#include "plugin_clap.h"

#include <clap/helpers/host-proxy.hh>
#include <clap/helpers/plugin.hh>
#include <clap/helpers/plugin.hxx>

#include <array>

// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays, modernize-avoid-c-arrays)
static constexpr const char *const kClapFeatures[] = {
  CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
  CLAP_PLUGIN_FEATURE_STEREO,
  nullptr
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
clap_plugin_descriptor ClapPlugin::descriptor = {
  .clap_version = CLAP_VERSION,
  .id = "dev.jorchime.mge",
  .name = "Minimal Gain Example",
  .vendor = "Jorchime",
  .url = "jorchime.com",
  .manual_url = "jorchime.com",
  .support_url = "jorchime.com",
  .version = "0.0.1",
  .description = "Minimal Gain Example",
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  .features = kClapFeatures
};

ClapPlugin::ClapPlugin(const clap_host* host) : ClapPluginBase(&descriptor, host) {}

ClapPlugin::~ClapPlugin() = default;

bool ClapPlugin::init() noexcept {
  parameters = std::make_unique<ParameterList<double>>();

  // NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores)
  auto id = parameters->createParameter(
    "Gain man!",
     0.0,
     1.0,
     0.8);
  parameterPresenters.emplace(id, std::make_unique<ClapParameterPresenter>());
  parameters->registerView(id, parameterPresenters[id].get());

  return true;
}

bool ClapPlugin::audioPortsInfo(uint32_t index, bool isInput, clap_audio_port_info* info) const noexcept {
  if (index != 0 ) {
    return false;
  }

  info->id = index;
  info->in_place_pair = CLAP_INVALID_ID;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  strncpy(info->name, "main", sizeof(info->name));
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  info->flags = CLAP_AUDIO_PORT_IS_MAIN
              | CLAP_AUDIO_PORT_SUPPORTS_64BITS
              | CLAP_AUDIO_PORT_PREFERS_64BITS;
  info->channel_count = 2;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  info->port_type = CLAP_PORT_STEREO;

  return true;
}

bool ClapPlugin::paramsInfo(uint32_t paramIndex, clap_param_info* info) const noexcept {
  auto poptional = parameters->getParameter(paramIndex);
  if (poptional.has_value()) {
    const Parameter<double>* const p = poptional.value();
    info->id = p->id;
    // Don't copy last character to guarantee null-termination
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    p->name.copy(info->name, sizeof(info->name)-1);
    info->min_value = p->min_value;
    info->max_value = p->max_value;
    info->default_value = p->default_value;
    // info->flags = ;
    return true;
  } else {
    return false;
  }
}

bool ClapPlugin::paramsValue(clap_id paramId, double* value) noexcept {
  if(paramId < parameterPresenters.size()) {
    *value = parameterPresenters[paramId]->getDisplayValue();
    return true;
  } else {
    return false;
  }
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
bool ClapPlugin::paramsValueToText(clap_id paramId, double value, char* display, uint32_t size) noexcept {
  std::string text = std::to_string(value);
  // Don't copy last character to guarantee null-termination
  text.copy(display, sizeof(char)*size - 1);
  return true;
}

bool ClapPlugin::paramsTextToValue(clap_id paramId, const char* display, double* value) noexcept {
  std::string text(display);
  *value = std::stod(text);
  return true;
}

clap_process_status ClapPlugin::process(const clap_process *process) noexcept {
  updateParametersFromGui(process);

  const uint32_t frameCount = process->frames_count;
  const uint32_t inputEventCount = process->in_events->size(process->in_events);
  uint32_t eventIndex = 0;
  uint32_t nextEventFrame = inputEventCount ? 0 : frameCount;

  // do per block stuff outside of loop
  for (uint32_t fr = 0; fr < frameCount; ) {

    while (eventIndex < inputEventCount && nextEventFrame == fr) {
      const clap_event_header_t *event = process->in_events->get(process->in_events, eventIndex);

      if (event->time != fr) {
        nextEventFrame = event->time;
        break;
      }

      if (event->space_id == CLAP_CORE_EVENT_SPACE_ID && event->type == CLAP_EVENT_PARAM_VALUE) {
       // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
       auto* valueEvent = reinterpret_cast<const clap_event_param_value_t*>(event);
       parameters->setParameterValue(valueEvent->param_id, valueEvent->value);
     }

      eventIndex++;
      if (eventIndex == inputEventCount) {
        nextEventFrame = frameCount;
        break;
      }
    }

    // do per sample stuff here
    for(uint32_t i = fr; i < nextEventFrame; i++) {
      for(uint32_t ports = 0; ports < process->audio_outputs_count; ports++)
      for (uint32_t ch = 0; ch < process->audio_outputs->channel_count; ++ch) {
        // audio signal goes here
        // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        double volume = 0.0;
        this->paramsValue(0, &volume);

        if (process->audio_inputs[ports].data32){
          const float in = process->audio_inputs[ports].data32[ch][i];
          const float out = in * static_cast<float>(volume);
          process->audio_outputs[ports].data32[ch][i] = out;
        } else {
          const double in = process->audio_inputs[ports].data64[ch][i];
          const double out = in * volume;
          process->audio_outputs[ports].data64[ch][i] = out;
        }
        // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      }
    }

    fr = nextEventFrame;
  }

  return CLAP_PROCESS_CONTINUE;
}
