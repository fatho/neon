cmake_minimum_required(VERSION 3.0)
project("Neon")

# global defintions
set(OS_NAME "Neon")
set(OS_VERSION_MAJOR "0")
set(OS_VERSION_MINOR "1")
set(OS_TARGET_TRIPLE "x86_64-neon-elf")
set(OS_COPYRIGHT "Copyright 2014 (c) Fabian Thorand")

set(KERNEL_NAME "neon")
set(KERNEL_EXECUTABLE "${KERNEL_NAME}.sys")

# default prefix
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist" CACHE PATH "Neon install prefix")

# build sub-projects
add_subdirectory(grub)
add_subdirectory(kernel)
