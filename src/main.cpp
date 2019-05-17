#include <iostream>
#include "calculator_engine.h"

using namespace std;

int main()
{
  CalculatorEngine calculatorEngine;
  std::string operation;
  double operandA;
  double operandB;
  double result;

  calculatorEngine.start();

  while (true) {
    cout << "Enter operation: ";
    cin >> operation;
    if (operation == "exit") {
      break;
    }
    if (!calculatorEngine.isOperationSupported(operation)) {
      cerr << "Operation not supported" << endl;
    }
    else {
      cout << "operandA: ";
      cin >> operandA;
      cout << "operandB: ";
      cin >> operandB;
      result = calculatorEngine.runOperation(operation, operandA, operandB);
      cout << "Result: " << result << endl;
    }
  }

  calculatorEngine.stop();

  return 0;
}
