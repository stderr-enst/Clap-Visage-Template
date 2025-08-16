#include "plugin_clap.h"

#include <clap/helpers/host-proxy.hh>
#include <clap/helpers/plugin.hh>
#include <clap/helpers/plugin.hxx>

#include <cstring>
#include <limits>

int ClapPlugin::pluginWidth() const {
  if (app_ == nullptr)
    return 0;

#if __APPLE__
  return app_->width();
#else
  return app_->nativeWidth();
#endif
}

int ClapPlugin::pluginHeight() const {
  if (app_ == nullptr)
    return 0;

#if __APPLE__
  return app_->height();
#else
  return app_->nativeHeight();
#endif
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void ClapPlugin::setPluginDimensions(int width, int height) {
  if (app_ == nullptr)
    return;
#if __APPLE__
  app_->setWindowDimensions(width, height);
#else
  app_->setNativeWindowDimensions(width, height);
#endif
}

#ifdef __linux__
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void ClapPlugin::onPosixFd(int fd, clap_posix_fd_flags_t flags) noexcept {
  if (app_ && app_->window())
    app_->window()->processPluginFdEvents();
}
#endif

bool ClapPlugin::guiIsApiSupported(const char* api, bool is_floating) noexcept {
  if (is_floating)
    return false;

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
#ifdef _WIN32
  if (strcmp(api, CLAP_WINDOW_API_WIN32) == 0)
    return true;
#elif __APPLE__
  if (strcmp(api, CLAP_WINDOW_API_COCOA) == 0)
    return true;
#elif __linux__
  if (strcmp(api, CLAP_WINDOW_API_X11) == 0)
    return true;
#endif
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

  return false;
}

bool ClapPlugin::guiCreate(const char* api, bool is_floating) noexcept {
  if (is_floating)
    return false;

  if (app_)
    return true;

  app_ = std::make_unique<MyGui>();

  // Register Knob-view to gain parameter
  parameters[0].registerView(app_->getKnob());

  app_->onWindowContentsResized() = [this] { _host.guiRequestResize(pluginWidth(), pluginHeight()); };

  return true;
}

void ClapPlugin::guiDestroy() noexcept {
#if __linux__
  if (app_ && app_->window() && _host.canUsePosixFdSupport())
    _host.posixFdSupportUnregister(app_->window()->posixFd());
#endif

  app_->close();
}

bool ClapPlugin::guiSetParent(const clap_window* window) noexcept {
  if (app_ == nullptr)
    return false;

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
  app_->show(window->ptr);

#if __linux__
  if (_host.canUsePosixFdSupport() && app_->window()) {
    int fd_flags = CLAP_POSIX_FD_READ | CLAP_POSIX_FD_WRITE | CLAP_POSIX_FD_ERROR;
    return _host.posixFdSupportRegister(app_->window()->posixFd(), fd_flags);
  }
#endif
  return true;
}

bool ClapPlugin::guiGetResizeHints(clap_gui_resize_hints_t* hints) noexcept {
  if (app_ == nullptr)
    return false;

  bool fixed_aspect_ratio = app_->isFixedAspectRatio();
  hints->can_resize_horizontally = true;
  hints->can_resize_vertically = true;
  hints->preserve_aspect_ratio = fixed_aspect_ratio;

  if (fixed_aspect_ratio) {
    hints->aspect_ratio_width = static_cast<uint32_t>(app_->height() * app_->aspectRatio());
    hints->aspect_ratio_height = static_cast<uint32_t>(app_->width());
  }
  return true;
}

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
bool ClapPlugin::guiAdjustSize(uint32_t* width, uint32_t* height) noexcept {
  if (app_ == nullptr)
    return false;

  app_->adjustWindowDimensions(width, height, true, true);
  return true;
}

bool ClapPlugin::guiSetSize(uint32_t width, uint32_t height) noexcept {
  if (app_ == nullptr)
    return false;

  assert(width <= std::numeric_limits<int>::max());
  assert(height <= std::numeric_limits<int>::max());

  setPluginDimensions(static_cast<int>(width), static_cast<int>(height));
  return true;
}

bool ClapPlugin::guiGetSize(uint32_t* width, uint32_t* height) noexcept {
  if (app_ == nullptr)
    return false;

  *width = pluginWidth();
  *height = pluginHeight();
  return true;
}
// NOLINTEND(bugprone-easily-swappable-parameters)

void ClapPlugin::updateParametersFromGui(const clap_process *process) noexcept {

  if( app_ != nullptr ) {
    if ( app_->getKnob()->isChanged() ){
      parameters[0].setModel(app_->getKnob()->getRotation());
      app_->getKnob()->unsetChanged();

      clap_event_param_value_t event = {};
      event.header.size = sizeof(event);
      event.header.time = 0;
      event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
      event.header.type = CLAP_EVENT_PARAM_VALUE;
      event.header.flags = 0;
      event.param_id = 0;
      event.cookie = nullptr;
      event.note_id = -1;
      event.port_index = -1;
      event.channel = -1;
      event.key = -1;
      event.value = parameters[event.param_id].getParameter()->value;
      process->out_events->try_push(process->out_events, &event.header);
    } else {
      // synchronize all views to update Gui elements
      for(auto& param : parameters){
        param.updateViews();
      }
    }
  }
}
