set(TARGET_NAME "engine")

add_library(${TARGET_NAME} SHARED
    "calculator_engine.cpp"
    "calculator_engine.h"
    "plugin_registry.cpp"
    "plugin_registry.h"
    "plugin_entry.cpp"
    "plugin_entry.h"
    "plugin_utils.cpp"
    "plugin_utils.h"
)

# all plugin libs MUST be installed in a specific directory
set_target_properties(${TARGET_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "$ENV{HOME}/Desktop/calculator/lib")

target_include_directories(${TARGET_NAME} PRIVATE
    "../api")

target_link_libraries(${TARGET_NAME}
    "dl"
    "-Wl,-rpath=$ENV{HOME}/Desktop/calculator/plugins"
    "api"
)


set(CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
