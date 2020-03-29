cmake_minimum_required(VERSION 3.0)
project(snake VERSION 1.0.0)

file(GLOB SOURCES src/*.h src/*.cpp)

if(BUILD MATCHES "GBA")
    file(GLOB PORT_SOURCES src/gba/* src/gba/SuperCrateBox-GBA/* img/small/*.c img/small/*.h)
    list(APPEND SOURCES ${PORT_SOURCES})
else()
    file(GLOB PORT_SOURCES src/sdl/*)
    list(APPEND SOURCES ${PORT_SOURCES})

    set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/cmake-modules)

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2.cmake")
        file(DOWNLOAD
            "https://github.com/tcbrindle/sdl2-cmake-scripts/raw/master/FindSDL2.cmake"
            "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2.cmake")
    endif()

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_image.cmake")
        file(DOWNLOAD
            "https://github.com/tcbrindle/sdl2-cmake-scripts/raw/master/FindSDL2_image.cmake"
            "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_image.cmake")
    endif()

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_ttf.cmake")
        file(DOWNLOAD
            "https://github.com/tcbrindle/sdl2-cmake-scripts/raw/master/FindSDL2_ttf.cmake"
            "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_ttf.cmake")
    endif()

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
    )

if(BUILD MATCHES "GBA")
    target_include_directories(${PROJECT_NAME}
        PRIVATE
        img/small/
        )

    target_compile_definitions(${PROJECT_NAME} PRIVATE GBA_BUILD=1)

    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${PROJECT_NAME} ${PROJECT_NAME}.gba)
else()
    target_include_directories(${PROJECT_NAME}
        SYSTEM PRIVATE
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        )

    target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        )
endif()
