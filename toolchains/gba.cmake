set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_C_COMPILER       "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER     "arm-none-eabi-g++")
set(CMAKE_AR               "arm-none-eabi-ar" CACHE FILEPATH "" FORCE)
set(BUILD                  GBA)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mthumb-interwork -mthumb -O2")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -mthumb-interwork -mthumb -specs=gba.specs")
