SET(CMAKE_SYSTEM_NAME "Generic")
SET(CMAKE_SYSTEM_VERSION 1)

SET(CMAKE_C_COMPILER "i686-elf-gcc")
SET(CMAKE_CXX_COMPILER "i686-elf-g++")
SET(CMAKE_C_FLAGS  ${CMAKE_C_FLAGS} "-ffreestanding")
SET(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} "-ffreestanding")
SET(CMAKE_EXE_LINKER_FLAGS  ${CMAKE_EXE_LINKER_FLAGS} "-nostdlib")

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS ON)