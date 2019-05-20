#ifndef PLUGIN_REGISTRY_H
#define PLUGIN_REGISTRY_H

#include <map>
#include <string>
#include "plugin_entry.h"

/**
 * Implements the plugin registry, which is used to register, discover, and 
 * load/unload plugins.
 */
class PluginRegistry
{

public:

  /**
   * Destructor.
   */
  ~PluginRegistry();
  
  /**
   * (Signleton pattern)
   * Returns the plugin registry shared instance.
   * 
   * @return The PluginRegistry shared instance
   */
  static PluginRegistry& getSharedInstance()
  {
    static PluginRegistry s_sharedInstance;
    return s_sharedInstance;
  }

  /**
   * Initializes the plugin registry.
   */
  void initialize();

  /**
   * Discovers the plugin with specified type and name.
   *
   * @param type The desired plugin type
   * @param name The desired plugin name
   *
   * @return A plugin entry corresponding to the dicsovered plugin, or nullptr
   */
  PluginEntry *get(std::string type, std::string name);

  /**
   * Loads the specified plugin.
   *
   * @param type The plugin type
   * @param name The plugin name
   *
   * @return A pointer to the plugin instance
   */
  void *loadPlugin(std::string type, std::string name);

  /**
   * Unloads the specified plugin.
   *
   * @param type The plugin type
   * @param name The plugin name
   * @param plugin Pointer to the plugin instance
   */
  void unloadPlugin(std::string type, std::string name, void *plugin);

private:

  /**
   * Constructor.
   */
  PluginRegistry();

  /**
   * The plugin registry entries.
   */
  std::map<std::string, std::map<std::string, PluginEntry*>> m_entries;

  /**
   * The map of plugin handles.
   */
  std::map<std::string, void*> m_pluginHandleMap;
};

#endif // PLUGIN_REGISTRY_H
