#ifndef PLUGIN_CLAP_H
#define PLUGIN_CLAP_H

#include "gui.h"
#include "IParameterView.h"
#include "ParameterList.h"

#include <clap/helpers/plugin.hh>
#include <clap/helpers/plugin.hxx>

class ClapParameterPresenter : public IParameterView<double> {
public:
  virtual ~ClapParameterPresenter() = default;

  void setView(const double& v) override {
    displayValue = v;
  }

  [[nodiscard]] const double getDisplayValue() const override {
    return displayValue;
  }

private:
  double displayValue{0.0};
};

using ClapPluginBase = clap::helpers::Plugin<clap::helpers::MisbehaviourHandler::Terminate,
                                             clap::helpers::CheckingLevel::Maximal>;

class ClapPlugin : public ClapPluginBase {
public:
  static clap_plugin_descriptor descriptor;

  explicit ClapPlugin(const clap_host* host);
  ~ClapPlugin() override;

  bool init() noexcept override;

#ifdef __linux__
  bool implementsPosixFdSupport() const noexcept override { return true; }
  void onPosixFd(int fd, clap_posix_fd_flags_t flags) noexcept override;
#endif

protected:
  clap_process_status process(const clap_process* process) noexcept override;

  bool implementsGui() const noexcept override { return true; }
  bool guiIsApiSupported(const char* api, bool is_floating) noexcept override;
  bool guiCreate(const char* api, bool is_floating) noexcept override;
  void guiDestroy() noexcept override;
  bool guiSetParent(const clap_window* window) noexcept override;
  bool guiSetScale(double scale) noexcept override { return false; }
  bool guiCanResize() const noexcept override { return true; }
  bool guiGetResizeHints(clap_gui_resize_hints_t* hints) noexcept override;
  bool guiAdjustSize(uint32_t* width, uint32_t* height) noexcept override;
  bool guiSetSize(uint32_t width, uint32_t height) noexcept override;
  bool guiGetSize(uint32_t* width, uint32_t* height) noexcept override;
  void updateParametersFromGui(const clap_process* process) noexcept;

  bool implementsAudioPorts() const noexcept override { return true; }
  uint32_t audioPortsCount(bool isInput) const noexcept override { return 1; }
  bool audioPortsInfo(uint32_t index, bool isInput, clap_audio_port_info* info) const noexcept override;

  bool implementsParams() const noexcept override { return true; }
  uint32_t paramsCount() const noexcept override { return 1; }
  bool paramsInfo(uint32_t paramIndex, clap_param_info* info) const noexcept override;
  bool paramsValue(clap_id paramId, double* value) noexcept override;
  bool paramsValueToText(clap_id paramId, double value, char* display, uint32_t size) noexcept override;
  bool paramsTextToValue(clap_id paramId, const char* display, double* value) noexcept override;

private:
  [[nodiscard]] int pluginWidth() const;
  [[nodiscard]] int pluginHeight() const;
  void setPluginDimensions(int width, int height);

  std::unique_ptr<MyGui> app_;
  std::unique_ptr<ParameterList<double>> parameters;
  std::unordered_map<unsigned int, std::unique_ptr<ClapParameterPresenter>> parameterPresenters;
};

#endif // PLUGIN_CLAP_H
