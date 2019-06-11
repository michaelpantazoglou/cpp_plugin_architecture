#ifndef ADDITION_PLUGIN_H
#define ADDITION_PLUGIN_H

#include "operation.h"
#include <string>

/**
 * Implements the addition operation plugin.
 */
class AdditionPlugin : public Operation
{

public:
  
  /**
   * Constructor.
   */
  AdditionPlugin();

  /**
   * Destructor.
   */
  ~AdditionPlugin();

  /**
   * Executes the addition operation.
   *
   * @param operandA The first operand
   * @param operandB The second operand
   *
   * @return The addition result
   */
  virtual double execute(double operandA, double operandB) override;
};

// The following methods are used by the plugin registry to retrieve the 
// plugin metadata. They are called via dlopen.

extern "C"
const char *getName()
{
  return "add";
}

extern "C"
Operation *create()
{
  return new AdditionPlugin();
}

extern "C"
void destroy(Operation *operation)
{
  delete reinterpret_cast<Operation*>(operation);
}

#endif // ADDITION_PLUGIN_H
