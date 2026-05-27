cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME The_Second_Task)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        BSP/Inc/bsp_adc.h
        BSP/Src/bsp_adc.c
        Core/Src/ili9341_driver.c
        Core/Src/font.c
        Core/Inc/ili9341_driver.h
        Core/Inc/font.h
        BSP/Inc/bsp_timer.h
        BSP/Src/bsp_timer.c
        BSP/Inc/bsp_dac.h
        BSP/Src/bsp_dac.c
        BSP/Src/algo_trigger.c
        BSP/Inc/algo_trigger.h
        BSP/Src/algo_measure.c
        BSP/Inc/algo_measure.h
        BSP/Src/app_ui.c
        BSP/Inc/app_ui.h
        BSP/Src/algo_dsp.c
        BSP/Inc/algo_dsp.h
        BSP/Src/app_auto.c
        BSP/Inc/app_auto.h
        BSP/Src/bsp_encoder.c
        BSP/Inc/bsp_encoder.h
        BSP/Src/app_calibration.c
        BSP/Inc/app_calibration.h
        BSP/Src/app_afe.c
        BSP/Inc/app_afe.h
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
    ${CMAKE_SOURCE_DIR}/Core/Lib/DSP/Lib
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    BSP/Src/bsp_adc.c
    Core/Src/ili9341_driver.c
    Core/Src/font.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    BSP/Inc
    Core/Lib/DSP/Include
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
    ARM_MATH_CM7
    __FPU_PRESENT=1
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    # Add user defined libraries
    arm_cortexM7lfdp_math m
)
