#ifndef CALCULATOR_ENGINE_H
#define CALCULATOR_ENGINE_H

#include <string>

/**
 * Implements a generic and extensible calculator engine.
 * The engine can be extended with plugins that implement arithmetic operations.
 * For instance, if we want the calculator engine to support the addition 
 * operation, we have to implement a corresponding Operation plugin.
 */
class CalculatorEngine
{
public:
  
  /**
   * Constructor.
   */
  CalculatorEngine();

  /**
   * Starts the calculator engine.
   * Internally, this method will initialize the plugin registry.
   */
  void start();

  /**
   * Stops the calculator engine.
   */
  void stop();

  /**
   * Checks if the operation identified by the given name is supported or not.
   *
   * @param name The operation name
   *
   * @return true if the operation is supported, otherwise false
   */
  bool isOperationSupported(std::string name);

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
  double runOperation(std::string name, double operandA, double operandB);
};

#endif // CALCULATOR_ENGINE_H
