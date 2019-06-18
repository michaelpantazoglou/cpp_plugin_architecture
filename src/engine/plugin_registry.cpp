#include "plugin_registry.h"
#include "plugin_utils.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>

/**
 * This is where the plugin registry expects to find the solidMediaEngine 
 * plugins. Each plugin corresponds to a .so file located under this directory.
 */
#define PLUGINS_HOMEDIR "/home/michaelp/Desktop/calculator/plugins"

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
    std::map<std::string, PluginEntry*>::const_iterator pluginEntryIter;
    for (pluginEntryIter = entries.begin(); pluginEntryIter != entries.end(); ++pluginEntryIter) {
      unloadPlugin(pluginEntryIter->second);
      delete pluginEntryIter->second;
    }
    entries.clear();
  }
  m_entries.clear();
}


/**
 * Initializes the plugin registry.
 */
void PluginRegistry::initialize()
{
  // By convention, the plugin registry expects that all plugin .so libraries 
  // are located under the specified folder
  std::string pluginsDir = PLUGINS_HOMEDIR;
  DIR* dirp = opendir(pluginsDir.c_str());
  if (NULL == dirp) {
    std::cerr << "Could not open directory: " << pluginsDir << std::endl;
    return;
  }

  struct dirent * dp;

  // Loop for each plugin library found at the specified folder
  std::cout << "Traversing directory " << pluginsDir << std::endl;
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
    void *plugin = PluginUtils::CreatePlugin(lib);
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
 * Gets all registered plugin entries.
 *
 * @return A vector that contains pointers to the PluginEntry instances
 *         corresponding to the registered plugins
 */
std::vector<PluginEntry*> PluginRegistry::getAll()
{
  std::vector<PluginEntry*> result;
  for (auto element : m_entries) {
    std::map<std::string, PluginEntry*> map = element.second;
    for (auto entry : map) {
      result.push_back(entry.second);
    }
  }
  return result;
}


/**
 * Loads the specified plugin.
 *
 * @param pluginEntry Pointer to the corresponding plugin entry
 *
 * @return A pointer to the plugin instance
 */
void *PluginRegistry::loadPlugin(PluginEntry *pluginEntry)
{
  if (nullptr == pluginEntry) {
    return nullptr;
  }

  std::string pluginId = pluginEntry->getId();

  // Check if there is already a handle for this plugin
  if (nullptr != m_pluginHandleMap[pluginId]) {
    return m_pluginHandleMap[pluginId];
  }

  // Open plugin library
  std::cout << "Loading library " << pluginEntry->getLibName() << std::endl;
  void *lib = PluginUtils::OpenPluginLibrary(pluginEntry->getLibName());
  if (!lib) {
    return nullptr;
  }

  // Create and return Operation plugin instance
  void *plugin = PluginUtils::CreatePlugin(lib);

  // Add handles to internal maps for reuse
  m_pluginHandleMap[pluginId] = plugin;
  m_pluginLibMap[pluginId] = lib;

  return plugin;
}


/**
 * Unloads the specified plugin.
 *
 * @param pluginEntry Pointer to the corresponding plugin entry
 */
void PluginRegistry::unloadPlugin(PluginEntry *pluginEntry)
{
  if (!pluginEntry) {
      return;
  }

  std::string pluginId = pluginEntry->getId();

  void *plugin = m_pluginHandleMap[pluginId];
  void *lib = m_pluginLibMap[pluginId];

  if (nullptr != lib) {
    if (nullptr != plugin) {
      PluginUtils::DestroyPlugin(lib, plugin);
      m_pluginHandleMap.erase(m_pluginHandleMap.find(pluginId));
    }

    PluginUtils::ClosePluginLibrary(lib);
    m_pluginLibMap.erase(m_pluginLibMap.find(pluginId));
    std::cout << "Plugin with id = " << pluginId << " successfully unloaded" << std::endl;
  }
}