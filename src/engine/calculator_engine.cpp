#include "calculator_engine.h"
#include "plugin_registry.h"
#include <iostream>
#include <assert.h>
#include <dlfcn.h>

using namespace std;

/**
 * Constructor.
 */
CalculatorEngine::CalculatorEngine()
{
}


/**
 * Starts the calculator engine.
 * Internally, this method will initialize the plugin registry.
 */
void CalculatorEngine::start()
{
  PluginRegistry::getSharedInstance().initialize();
  cout << "Calculator engine started" << endl;
}


/**
 * Stops the calculator engine.
 */
void CalculatorEngine::stop()
{
  cout << "Calculator engine stopped" << endl;
}


/**
 * Checks if the operation identified by the given name is supported or not.
 *
 * @param name The operation name
 *
 * @return true if the operation is supported, otherwise false
 */
bool CalculatorEngine::isOperationSupported(std::string name)
{
  PluginEntry *pluginEntry = PluginRegistry::getSharedInstance().get("operation", name);
  if (!pluginEntry) {
    return false;
  }
  return true;
}


/**
 * Runs the operation identified by the given name, with the two specified
 * operands. This method will call the plugin that corresponds to the given 
 * operation name to actually carry out the task.
 *
 * @param name The operation name
 * @param operandA The first operand
 * @param operandB The second operand
 *
 * @return The operation result
 */
double CalculatorEngine::runOperation(std::string name, double operandA, double operandB)
{
  // Discover the requested operation plugin by name
  PluginEntry *pluginEntry = PluginRegistry::getSharedInstance().get("operation", name);
  if (!pluginEntry) {
    return -1;
  }
  
  // dlopen the plugin library
  void *lib = dlopen(pluginEntry->getLibName().c_str(), RTLD_LAZY);
  if (!lib) {
    std::cerr << "dlopen failed" << std::endl;
    return -1;
  }

  // Create plugin instance
  dlerror();
  createInstance_t *create = reinterpret_cast<createInstance_t *>(dlsym(lib, "create"));
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "dlerror: " << dlsym_error << std::endl;
    dlclose(lib);
    return -1;
  }
  Operation *plugin = create();
  
  // Execute the plugin
  double result = plugin->execute(operandA, operandB);

  // Destroy plugin and dlclose the plugin library
  destroyInstance_t *destroy = reinterpret_cast<destroyInstance_t *>(dlsym(lib, "destroy"));
  dlerror();
  destroy(plugin);
  dlclose(lib);

  // Finally, return the operation result
  return result;
}
