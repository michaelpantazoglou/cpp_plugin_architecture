#include "plugin_registry.h"
#include <dlfcn.h>
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
 * Initializes the plugin registry.
 */
void PluginRegistry::initialize()
{
  // By convention, the plugin registry expects that all plugin .so libraries 
  // are located under the specified folder
  const std::string dirname = "~/Desktop/plugins";
  DIR* dirp = opendir(dirname.c_str());
  struct dirent * dp;

  while ((dp = readdir(dirp)) != nullptr) {
    std::string libname = dp->d_name;

    // avoid '.' and '..' entries
    if (libname == "." || libname == "..") {
      continue;
    }

    std::string fullpath = libname;

    std::cout << "Attempting to dlopen lib " << fullpath << " ..." << std::endl;

    void *lib = dlopen(fullpath.c_str(), RTLD_LAZY);
    const char* dlsym_error = dlerror();
    if (!lib) {
      std::cerr << "Cannot open lib";
      if (dlsym_error) {
        std::cerr << ": " << dlsym_error;
      }
      std::cerr << std::endl;
      continue;
    }

    dlerror();

    std::cout<< "Lib dlopened" << std::endl;

    // Create plugin instance in order to resolve its metadata.
    createInstance_t *create = reinterpret_cast<createInstance_t *>(dlsym(lib, "create"));
    dlsym_error = dlerror();
    if (dlsym_error) {
      std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
      dlclose(lib);
      continue;
    }
    create();
    create = nullptr;

    // Resolve the plugin type
    getType_t *getPluginType = reinterpret_cast<getType_t *>(dlsym(lib, "getType"));
    dlsym_error = dlerror();
    if (dlsym_error) {
      std::cerr << "Cannot load symbol getType: " << dlsym_error << std::endl;
      dlclose(lib);
      continue;
    }

    // Resolve the plugin name
    getName_t *getPluginName = reinterpret_cast<getName_t *>(dlsym(lib, "getName"));
    dlsym_error = dlerror();
    if (dlsym_error) {
      std::cerr << "Cannot load symbol getName: " << dlsym_error << std::endl;
      dlclose(lib);
      continue;
    }
    std::cout << "Got all required symbols" << std::endl;

    std::string pluginType = getPluginType();
    std::string pluginName = getPluginName();

    std::cout << "pluginType=" << pluginType << ", pluginName=" << pluginName << std::endl;

    // Create the corresponding plugin entry and populate its properties
    // Then, add the plugin entry to the registry
    PluginEntry *pluginEntry = new PluginEntry(pluginType, pluginName, libname);
    std::pair<std::string, PluginEntry*> p(pluginName, pluginEntry);
    m_entries[pluginType].insert(p);

    std::cout << "Added plugin (type=" << pluginType << ", name=" << pluginName << ")" << std::endl;
    dlclose(lib);
  }
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
 * Loads the specified operation plugin.
 *
 * @param name The operation plugin name
 *
 * @return A pointer to the Operation plugin instance
 */
void *lib = nullptr;
Operation *PluginRegistry::loadOperationPlugin(std::string name)
{
  // Sanity check that the plugin exists
  PluginEntry *pluginEntry = get("operation", name);
  if (!pluginEntry) {
    return nullptr;
  }

  // dlopen plugin library
  std::cout << "Loading library " << pluginEntry->getLibName() << std::endl;
  lib = dlopen(pluginEntry->getLibName().c_str(), RTLD_LAZY);
  if (!lib) {
    std::cerr << "dlopen failed" << std::endl;
    return nullptr;
  }

  // Create plugin instance
  dlerror();
  createInstance_t *create = reinterpret_cast<createInstance_t *>(dlsym(lib, "create"));
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "dlerror: " << dlsym_error << std::endl;
    dlclose(lib);
    return nullptr;
  }
  std::cout << "creating operation plugin instance" << std::endl;
  Operation *plugin = create();
  std::cout << "plugin instance successfully created" << std::endl;
  
  // Return plugin instance
  return plugin;
}


/**
 * Unloads the specified plugin.
 *
 * @param name The plugin nam
 * @param plugin Pointer to the plugin instance
 */
void PluginRegistry::unloadOperationPlugin(std::string name, Operation *plugin)
{
  // Sanity check that the plugin exists
  PluginEntry *pluginEntry = get("operation", name);
  if (!pluginEntry) {
    return;
  }
  
  // Call the destroy() method of the plugin and dlclose its previously dlopened library
  destroyInstance_t *destroy = reinterpret_cast<destroyInstance_t *>(dlsym(lib, "destroy"));
  dlerror();
  destroy(plugin);
  dlclose(lib);
}
