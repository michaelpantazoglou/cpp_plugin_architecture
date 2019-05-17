#include "addition_plugin.h"
#include <iostream>

/**
 * Constructor.
 */
AdditionPlugin::AdditionPlugin()
  : Operation()
{
}


/**
 * Destructor.
 */
AdditionPlugin::~AdditionPlugin()
{
}


/**
 * Executes the addition operation.
 *
 * @param operandA The first operand
 * @param operandB The second operand
 *
 * @return The addition result
 */
double AdditionPlugin::execute(double operandA, double operandB)
{
  return operandA + operandB;
}
