#ifndef ABSTRACT_PLUGIN_H
#define ABSTRACT_PLUGIN_H

#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/**
 * This class implements the basic plugin abstraction. All plugin interfaces
 * must override this base class.
 */
class AbstractPlugin 
{

public:

  /**
   * Invokes the specified plugin method using the specified JSON message
   * as input.
   *
   * @param methodName The name of the method to be invoked
   * @param input A JSON message containing the method's input parameters
   *
   * @return A JSON message containing the method's output (if any)
   */
  virtual json invokeMethod(std::string methodName, json input) = 0;
};

#endif