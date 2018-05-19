# Create a library called bcm238x-arm-peripheral

# Source files:
#       src/bcm283x-arm-peripheral.cxx
# Header files:
#       include/bcm283x-arm-peripheral.h
cmake_minimum_required(VERSION 3.1)

# Use standard c++14
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup to cross compile for ARM
set(CMAKE_SYSTEM_NAME Linux)  
set(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_AR arm-linux-gnueabihf-ar CACHE FILEPATH "Archiver")
set(CMAKE_FIND_ROOT_PATH /home/paul/Projects/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


add_library(bcm283x-arm-peripheral ${CMAKE_CURRENT_SOURCE_DIR}/src/bcm283x-arm-peripheral.cxx)

# Ensure compiler can find include files for this library
# for consumers of this library
target_include_directories(bcm283x-arm-peripheral PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


