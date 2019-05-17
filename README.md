# C++ Plugin Architecture Demo

## Introduction
This project implements the plugin architecture in the C++ programming language and demonstrates its use through a demo application.

## Build Instructions
The project build has been tested in Ubuntu 16.04 LTS. 

From a terminal, navigate to the project root directory and type:

```console
mkdir build && cd build
cmake ..
make
```

The following directories and contents will be created upon successful build:

```
~/Desktop/lib/libapi.so
~/Desktop/lib/libengine.so
~/Desktop/plugins/libaddition_plugin.so
~/Desktop/plugins/libsubtraction_plugin.so
```

## Demo Execution
While in build directory, type:

```console
./calculator
```

### Example input/output
The following is an example of the input and output of the calculator application:

```
Attempting to dlopen lib libsubtraction_plugin.so ...
Lib dlopened
Got all required symbols
pluginType=operation, pluginName=sub
Added plugin (type=operation, name=sub)
Attempting to dlopen lib libaddition_plugin.so ...
Lib dlopened
Got all required symbols
pluginType=operation, pluginName=add
Added plugin (type=operation, name=add)
Calculator engine started
Enter operation: add
operandA: 1
operandB: 2
Result: 3
Enter operation: multiply
Operation not supported
Enter operation: sub     
operandA: 1
operandB: 4565
Result: -4564
Enter operation: exit
Calculator engine stopped
```

## Plugin Development

For example, to create a plugin for the multiplication operation:

1. Create a plugin_multiplication directory under src
1. Create CMakeLists.txt, multiplication_plugin.h and multiplication_plugin.cpp under plugin_multiplication
1. Implement those files in a similar way as e.g. the contents of plugin_addition folder
1. Open top-level src/CMakeLists.txt and add:

```cmake
add_subdirectory("src/plugin3")
```

1. Re-build the project
1. The plugin should be now available to the calculator engine