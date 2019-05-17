#ifndef OPERATION_H
#define OPERATION_H

#include <string>

/**
 * This abstract class defines the interface of the Operation plugin.
 */
class Operation {

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
   * Returns the interface version.
   *
   * @return The version
   */
  std::string version() { return "1.0"; }
};

// The following operation typedefs are used by the plugin registry to 
// interact with the plugins through dlopen.
typedef Operation *createInstance_t();
typedef void destroyInstance_t(Operation*);
typedef const char *getType_t();
typedef const char *getName_t();

#endif // OPERATION_H
