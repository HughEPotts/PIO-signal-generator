# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(square50_example C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
# also need to add any other files manually - square50.cpp in this case
add_executable(square50_example square50_example.cpp square50.cpp )

pico_set_program_name(square50_example "square50_example")
pico_set_program_version(square50_example "0.1")

# Generate PIO header
pico_generate_pio_header(square50_example ${CMAKE_CURRENT_LIST_DIR}/square50.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(square50_example 0)
pico_enable_stdio_usb(square50_example 1)

# Add the standard library to the build
target_link_libraries(square50_example
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(square50_example PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(square50_example 
        hardware_pio
        )

pico_add_extra_outputs(square50_example)

