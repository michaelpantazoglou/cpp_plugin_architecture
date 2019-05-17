#include "subtraction_plugin.h"
#include <iostream>

/**
 * Constructor.
 */
SubtractionPlugin::SubtractionPlugin()
  : Operation()
{
}


/**
 * Destructor.
 */
SubtractionPlugin::~SubtractionPlugin()
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
double SubtractionPlugin::execute(double operandA, double operandB)
{
  return operandA - operandB;
}
