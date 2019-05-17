#ifndef PLUGIN_ENTRY_H
#define PLUGIN_ENTRY_H

#include <string>
#include "operation.h"

/**
 * This class is used to represent a plugin within the plugin registry.
 */
class PluginEntry
{
public:
  
  /**
   * Constructor.
   *
   * @param type The plugin type
   * @param name The plugin name
   * @param libName The plugin library name
   */
  PluginEntry(std::string type, std::string name, std::string libName);

  /**
   * Destructor.
   */
  ~PluginEntry();

  /**
   * Gets the plugin type.
   *
   * @return The plugin type
   */
  std::string getType() const;
  
  /**
   * Gets the plugin name.
   *
   * @return The plugin name
   */
  std::string getName() const;
  
  /**
   * Gets the plugin library name.
   *
   * @return The plugin library name
   */
  std::string getLibName() const;


private:

  /**
   * The plugin type.
   */
  std::string m_type;
  
  /**
   * The plugin name.
   */
  std::string m_name;
  
  /**
   * The plugin library name.
   */
  std::string m_libName;
};

#endif // PLUGIN_ENTRY_H
