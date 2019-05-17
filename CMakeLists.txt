cmake_minimum_required(VERSION 2.8)

project(plugin_architecture)

set(TARGET_NAME "calculator")

add_executable(${TARGET_NAME}
    "src/main.cpp"
)

target_include_directories(${TARGET_NAME} PRIVATE
    "src/engine"
    "src/api"
)

target_link_libraries(${TARGET_NAME}
    "-Wl,-rpath=/home/michaelp/Desktop/plugins/"
    "-Wl,-rpath=/home/michaelp/Desktop/lib/"
    "engine"
    "api"
)

add_subdirectory("src/engine")
add_subdirectory("src/api")
add_subdirectory("src/plugin_addition")
add_subdirectory("src/plugin_subtraction")

set(CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
