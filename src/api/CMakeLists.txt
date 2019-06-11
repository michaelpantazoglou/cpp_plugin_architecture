set(TARGET_NAME "api")

add_library(${TARGET_NAME} SHARED 
  "abstract_plugin.h"
  "operation.h"
  )

target_include_directories(${TARGET_NAME} PRIVATE 
  "../json"
  )

set_target_properties(${TARGET_NAME} PROPERTIES
    LINKER_LANGUAGE CXX
    LIBRARY_OUTPUT_DIRECTORY "$ENV{HOME}/Desktop/calculator/lib")
