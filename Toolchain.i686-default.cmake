SET(CMAKE_SYSTEM_NAME "Generic")
SET(CMAKE_SYSTEM_VERSION 1)

SET(triple i686)
SET(CMAKE_C_COMPILER "clang")
SET(CMAKE_CXX_COMPILER "clang")
SET(CMAKE_C_COMPILER_ID "Clang")
SET(CMAKE_CXX_COMPILER_ID "Clang")
SET(CMAKE_C_COMPILER_TARGET ${triple})
SET(CMAKE_CXX_COMPILER_TARGET ${triple})


SET(CMAKE_C_FLAGS  "--target=${triple} -Wall -Wextra -ffreestanding")
SET(CMAKE_CXX_FLAGS  "--target=${triple} ${CMAKE_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS_INIT  "--target=${triple} ${CMAKE_EXE_LINKER_FLAGS_INIT}")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)