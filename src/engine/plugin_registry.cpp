#include "plugin_registry.h"
#include "plugin_utils.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>

/**
 * Constructor.
 */
PluginRegistry::PluginRegistry()
{
}


/**
 * Destructor.
 */
PluginRegistry::~PluginRegistry()
{
  std::cout << "Clearing plugin registry" << std::endl;
  std::map<std::string, std::map<std::string, PluginEntry*>>::const_iterator pluginType;
  for (pluginType = m_entries.begin(); pluginType != m_entries.end(); ++pluginType) {
    std::map<std::string, PluginEntry*> entries = pluginType->second;
    std::map<std::string, PluginEntry*>::const_iterator pluginEntry;
    for (pluginEntry = entries.begin(); pluginEntry != entries.end(); ++pluginEntry) {
      delete pluginEntry->second;
    }
    entries.clear();
  }
  m_entries.clear();

  std::map<std::string, void*>::const_iterator handle;
  for (handle = m_pluginHandleMap.begin(); handle != m_pluginHandleMap.end(); ++handle) {
    PluginUtils::ClosePluginLibrary(handle->second);
  }
  m_pluginHandleMap.clear();
}


/**
 * Initializes the plugin registry.
 */
void PluginRegistry::initialize()
{
  // By convention, the plugin registry expects that all plugin .so libraries 
  // are located under the specified folder
  std::string homeDir(getenv("HOME"));
  std::string pluginsDir = homeDir + "/Desktop/calculator_engine/plugins";
  DIR* dirp = opendir(pluginsDir.c_str());
  struct dirent * dp;

  // Loop for each plugin library found at the specified folder
  while ((dp = readdir(dirp)) != nullptr) {
    std::string libname = dp->d_name;

    // Make sure to avoid '.' and '..' entries
    if (libname == "." || libname == "..") {
      continue;
    }

    std::string fullpath = libname;
    std::cout << "Attempting to open lib " << fullpath << " ..." << std::endl;

    // Open plugin library
    void *lib = PluginUtils::OpenPluginLibrary(fullpath);
    if (nullptr == lib) {
      continue;
    }

    // Create plugin instance in order to resolve its metadata.
    Operation *plugin = reinterpret_cast<Operation*>(PluginUtils::CreatePlugin(lib));
    if (nullptr == plugin) {
      PluginUtils::ClosePluginLibrary(lib);
      continue;
    }

    // Resolve the plugin type
    std::string pluginType = PluginUtils::GetPluginType(lib);
    if (pluginType.empty()) {
      PluginUtils::ClosePluginLibrary(lib);
      continue;
    }

    // Resolve the plugin name
    std::string pluginName = PluginUtils::GetPluginName(lib);
    if (pluginName.empty()) {
      PluginUtils::ClosePluginLibrary(lib);
      continue;
    }

    // Destroy plugin instance
    PluginUtils::DestroyPlugin(lib, plugin);

    // Close plugin library
    PluginUtils::ClosePluginLibrary(lib);

    // Create the corresponding plugin entry and populate its properties
    // Then, add the plugin entry to the registry
    PluginEntry *pluginEntry = new PluginEntry(pluginType, pluginName, libname);
    std::pair<std::string, PluginEntry*> p(pluginName, pluginEntry);
    m_entries[pluginType].insert(p);

    std::cout << "Added plugin (type=" << pluginType << ", name=" << pluginName << ")" << std::endl;
  }

  free(dp);
  closedir(dirp);
}


/**
 * Discovers the plugin with specified type and name.
 *
 * @param type The desired plugin type
 * @param name The desired plugin name
 *
 * @return A plugin entry corresponding to the dicsovered plugin, or nullptr
 */
PluginEntry *PluginRegistry::get(std::string type, std::string name)
{
  std::map<std::string, PluginEntry*> m = m_entries[type];
  return m[name];
}


/**
 * Loads the specified plugin.
 *
 * @param type The plugin type
 * @param name The plugin name
 *
 * @return A pointer to the plugin instance
 */
void *PluginRegistry::loadPlugin(std::string type, std::string name)
{
  // Check if there is already a handle for this plugin type
  if (nullptr != m_pluginHandleMap[type]) {
    return m_pluginHandleMap[type];
  }

  // Sanity check that the plugin exists
  PluginEntry *pluginEntry = get(type, name);
  if (!pluginEntry) {
    return nullptr;
  }

  // Open plugin library
  std::cout << "Loading library " << pluginEntry->getLibName() << std::endl;
  void *lib = PluginUtils::OpenPluginLibrary(pluginEntry->getLibName());
  if (!lib) {
    return nullptr;
  }

  // Add handle to internal map for reuse
  m_pluginHandleMap[type] = lib;

  // Create and return Operation plugin instance
  void *plugin = PluginUtils::CreatePlugin(lib);
  return plugin;
}


/**
 * Unloads the specified plugin.
 *
 * @param type The plugin type
 * @param name The plugin name
 * @param plugin Pointer to the plugin instance
 */
void PluginRegistry::unloadPlugin(std::string type, std::string name, void *plugin)
{
  // Sanity check that the plugin exists
  PluginEntry *pluginEntry = get(type, name);
  if (!pluginEntry) {
    return;
  }
  
  PluginUtils::DestroyPlugin(m_pluginHandleMap[type], plugin);
  PluginUtils::ClosePluginLibrary(m_pluginHandleMap[type]);

  m_pluginHandleMap.erase(m_pluginHandleMap.find(type));
}
