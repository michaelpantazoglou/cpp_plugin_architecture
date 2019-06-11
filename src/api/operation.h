#ifndef OPERATION_H
#define OPERATION_H

#include <string>
#include "abstract_plugin.h"

/**
 * This abstract class defines the interface of the Operation plugin.
 */
class Operation : public AbstractPlugin {

public:

  /**
   * Constructor.
   */
  Operation() {}
  
  /**
   * Destructor.
   */
  virtual ~Operation() {}
  
  /**
   * Executes this operation using the two given operands.
   *
   * @param operandA The first operand
   * @param operandB The second operand
   *
   * @return The operation result
   */
  virtual double execute(double operandA, double operandB) = 0;

  /**
   * Invokes the specified plugin method using the specified JSON message
   * as input.
   *
   * @param methodName The name of the method to be invoked
   * @param input A JSON message containing the method's input parameters
   *
   * @return A JSON message containing the method's output (if any)
   */
  virtual json invokeMethod(std::string methodName, json input) final
  {
    if ("execute" == methodName) {
      double operandA = input["operandA"].get<double>();
      double operandB = input["operandB"].get<double>();
      json output;
      double result = execute(operandA, operandB);
      output["result"] = result;
      return output;
    }
  }
};


/**
 * Gets the plugin type that corresponds to this interface.
 *
 * @return The plugin type
 */
extern "C"
const char *getType()
{
  return "operation";
}

#endif // OPERATION_H
