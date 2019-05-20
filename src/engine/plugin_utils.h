#ifndef PLUGIN_UTILS_H
#define PLUGIN_UTILS_H

#include <string>

/**
 * This class provides various plugin-related utility methods.
 */
class PluginUtils
{

public:

  /**
   * Opens the plugin library located at the specified path.
   * 
   * @param path The plugin library path
   * 
   * @return The library handle or nullptr
   */
  static void *OpenPluginLibrary(std::string path);

  /**
   * Closes the given plugin library.
   * 
   * @param pluginLib The plugin library to be closed
   */
  static void ClosePluginLibrary(void *pluginLib);

  /**
   * Creates the plugin instance that corresponds to the given lib.
   * 
   * @param pluginLib The dlopened plugin library
   * 
   * @return The plugin instance, or nullptr
   */
  static void *CreatePlugin(void *pluginLib);

  /**
   * Gets the type of the plugin that corresponds to the given lib.
   * 
   * @param pluginLib The dlopened plugin library
   * 
   * @return The plugin type, or an empty string
   */
  static std::string GetPluginType(void *pluginLib);

  /**
   * Gets the name of the plugin that corresponds to the given lib.
   * 
   * @param pluginLib The dlopened plugin library
   * 
   * @return The plugin name, or an empty string
   */
  static std::string GetPluginName(void *pluginLib);

  /**
   * Destroys the given plugin instance that corresponds to the given lib.
   * 
   * @param pluginLib The dlopened plugin library
   * @param plugin The plugin instance to be destroyed
   * 
   * @return true in success, otherwise false
   */
  static bool DestroyPlugin(void *pluginLib, void *plugin);

  typedef void *createInstance_t();
  typedef void destroyInstance_t(void*);
  typedef const char *getType_t();
  typedef const char *getName_t();

};

#endif // PLUGIN_UTILS_H