cmake_minimum_required(VERSION 4.0)
project(IO)

set(CMAKE_CXX_STANDARD 20)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Optional Wayland toggle
option(WAYLAND "Enable Wayland support via DBus portal" ON)

add_library(${PROJECT_NAME} SHARED
        include/LibIO/keyboard/KeyboardControls.hpp

        include/LibIO/mouse/MouseControls.hpp
        include/LibIO/mouse/Windows.hpp
        include/LibIO/mouse/Linux.hpp

        src/keyboard/Windows.cpp
        src/keyboard/Linux.cpp

        src/mouse/MouseControls.cpp
        src/mouse/Windows.cpp
        src/mouse/Linux.cpp
        library.cpp
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Platform detection
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "🖥️ Detected Windows platform")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "🐧 Detected Linux platform")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
else()
    message(WARNING "⚠️ Unknown platform: ${CMAKE_SYSTEM_NAME}")
endif()

include(X11)