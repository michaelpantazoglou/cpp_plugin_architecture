#include "plugin_utils.h"
#include <dlfcn.h>
#include <iostream>


/**
 * Opens the plugin library located at the specified path.
 * 
 * @param path The plugin library path
 * 
 * @return The library handle or nullptr
 */
void *PluginUtils::OpenPluginLibrary(std::string path)
{
  dlerror();
  void *lib = dlopen(path.c_str(), RTLD_LAZY);
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot open lib at '" << path << "': " << dlsym_error << std::endl;
    return nullptr;
  }
  return lib;
}


/**
 * Closes the given, previously opened plugin library.
 * 
 * @param pluginLib The plugin library to be closed
 */
void PluginUtils::ClosePluginLibrary(void *pluginLib)
{
  if (nullptr == pluginLib) {
    return;
  }
  dlerror();
  dlclose(pluginLib);
  pluginLib = nullptr;
}


/**
 * Creates the plugin instance that corresponds to the given lib.
 * 
 * @param pluginLib The plugin library
 * 
 * @return The Operation plugin instance, or nullptr
 */
void *PluginUtils::CreatePlugin(void *pluginLib)
{
  void *plugin = nullptr;

  if (nullptr == pluginLib) {
    std::cerr << "Plugin library was not dlopened" << std::endl;
    return plugin;
  }

  dlerror();
  createInstance_t *create = reinterpret_cast<createInstance_t *>(dlsym(pluginLib, "create"));
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
    return plugin;
  }
  
  plugin = create();
  return plugin;
}


/**
 * Gets the type of the plugin that corresponds to the given lib.
 * 
 * @param pluginLib The dlopened plugin library
 * 
 * @return The plugin type, or an empty string
 */
std::string PluginUtils::GetPluginType(void *pluginLib)
{
  std::string type;

  if (nullptr == pluginLib) {
    std::cerr << "Plugin library was not dlopened" << std::endl;
    return type;
  }

  dlerror();
  getType_t *getPluginType = reinterpret_cast<getType_t *>(dlsym(pluginLib, "getType"));
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol getType: " << dlsym_error << std::endl;
    return type;
  }

  type = getPluginType();
  return type;
}


/**
 * Gets the name of the plugin that corresponds to the given lib.
 * 
 * @param pluginLib The dlopened plugin library
 * 
 * @return The plugin name, or an empty string
 */
std::string PluginUtils::GetPluginName(void *pluginLib)
{
  std::string name;

  if (nullptr == pluginLib) {
    std::cerr << "Plugin library was not dlopened" << std::endl;
    return name;
  }

  dlerror();
  getName_t *getPluginName = reinterpret_cast<getName_t *>(dlsym(pluginLib, "getName"));
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol getName: " << dlsym_error << std::endl;
    return name;
  }

  name = getPluginName();
  return name;
}


/**
 * Destroys the given plugin instance that corresponds to the given lib.
 * 
 * @param pluginLib The dlopened plugin library
 * @param plugin The plugin instance to be destroyed
 * 
 * @return true in success, otherwise false
 */
bool PluginUtils::DestroyPlugin(void *pluginLib, void *plugin)
{
  dlerror();
  destroyInstance_t *destroy = reinterpret_cast<destroyInstance_t *>(dlsym(pluginLib, "destroy"));
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol destroy: " << dlsym_error << std::endl;
    return false;
  }
  destroy(plugin);
  return true;
}