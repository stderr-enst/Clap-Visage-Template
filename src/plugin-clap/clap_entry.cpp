#include "plugin_clap.h"

#include <cstring>

// NOLINTBEGIN(modernize-use-designated-initializers, cppcoreguidelines-pro-bounds-array-to-pointer-decay)
namespace plugin_entry {
  uint32_t getPluginCount(const clap_plugin_factory* f) {
    return 1;
  }

  const clap_plugin_descriptor* getPluginDescriptor(const clap_plugin_factory* f, uint32_t w) {
    return &ClapPlugin::descriptor;
  }

  static const clap_plugin* createPlugin(const clap_plugin_factory* factory,
                                         const clap_host* host,
                                         const char* plugin_id) {
    if (strcmp(plugin_id, ClapPlugin::descriptor.id) != 0)
      return nullptr;

    auto p = new ClapPlugin(host); // NOLINT(cppcoreguidelines-owning-memory)
    return p->clapPlugin();
  }

  static const void* get_factory(const char* factory_id) {
    static constexpr clap_plugin_factory clap_plugin_factory = {
      plugin_entry::getPluginCount,
      plugin_entry::getPluginDescriptor,
      plugin_entry::createPlugin,
    };
    return (!strcmp(factory_id, CLAP_PLUGIN_FACTORY_ID)) ? &clap_plugin_factory : nullptr;
  }

  bool clap_init(const char* p) {
    return true;
  }

  void clap_deinit() { }
}

extern "C" {
  extern const CLAP_EXPORT clap_plugin_entry clap_entry = {
    CLAP_VERSION,
    plugin_entry::clap_init,
    plugin_entry::clap_deinit,
    plugin_entry::get_factory
  };
}
// NOLINTEND(modernize-use-designated-initializers, cppcoreguidelines-pro-bounds-array-to-pointer-decay)
