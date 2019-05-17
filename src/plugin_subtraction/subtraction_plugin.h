#ifndef SUBTRACTION_PLUGIN_H
#define SUBTRACTION_PLUGIN_H

#include "operation.h"
#include <string>

/**
 * Implements the subtraction operation plugin.
 */
class SubtractionPlugin : public Operation
{

public:
  
  /**
   * Constructor.
   */
  SubtractionPlugin();

  /**
   * Destructor.
   */
  ~SubtractionPlugin();

  /**
   * Executes the subtraction operation.
   *
   * @param operandA The first operand
   * @param operandB The second operand
   *
   * @return The subtraction result
   */
  virtual double execute(double operandA, double operandB) override;
};

// The following methods are used by the plugin registry to retrieve the 
// plugin metadata. They are called via dlopen.

extern "C"
const char *getType()
{
  return "operation";
}

extern "C"
const char *getName()
{
  return "sub";
}

extern "C"
Operation *create()
{
  return new SubtractionPlugin();
}

extern "C"
void destroy(Operation *operation)
{
  delete reinterpret_cast<Operation*>(operation);
}

#endif // SUBTRACTION_PLUGIN_H
