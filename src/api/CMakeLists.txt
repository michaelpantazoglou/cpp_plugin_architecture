set(TARGET_NAME "api")

add_library(${TARGET_NAME} SHARED "operation.h")

set_target_properties(${TARGET_NAME} PROPERTIES
    LINKER_LANGUAGE CXX
    LIBRARY_OUTPUT_DIRECTORY "$ENV{HOME}/Desktop/calculator_engine/lib")
