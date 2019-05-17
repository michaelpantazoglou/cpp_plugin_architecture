set(TARGET_NAME "subtraction_plugin")

add_library(${TARGET_NAME} SHARED
    "subtraction_plugin.cpp"
    "subtraction_plugin.h"
    #"../api/operation.h"
)

#add_dependencies(${TARGET_NAME} api)

target_include_directories(${TARGET_NAME} PRIVATE "../api")

# all plugin libs MUST be installed in a specific directory
set_target_properties(${TARGET_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "~/Desktop/plugins"
    ARCHIVE_OUTPUT_DIRECTORY "~/Desktop/plugins")

target_link_libraries(${TARGET_NAME}
    "api"
    "-Wl,-rpath=/home/michaelp/Desktop/lib/"
)

set(CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
