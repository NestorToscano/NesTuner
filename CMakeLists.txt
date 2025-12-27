cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Import the Pico SDK (uses PICO_SDK_PATH from environment)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(lcd_display C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(lcd_display
    src/main.c
    src/ssd1306.c
    src/font5x7.c
    src/tuner.c
)

# Include directory for headers
target_include_directories(lcd_display PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
)

# include both stdlib and i2c libraries
target_link_libraries(lcd_display 
    pico_stdlib
    hardware_i2c
    hardware_adc
)

pico_enable_stdio_usb(lcd_display 1)
pico_add_extra_outputs(lcd_display)
