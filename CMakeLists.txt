cmake_minimum_required(VERSION 3.16)
project(LibIO VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---------------------------------------------------------
# Library
# ---------------------------------------------------------
add_library(${PROJECT_NAME} STATIC
        # Public headers
        include/LibIO/LibIO.hpp

        include/LibIO/keyboard/KeyboardControls.hpp
        include/LibIO/keyboard/Windows.hpp
        include/LibIO/keyboard/Linux.hpp

        include/LibIO/clipboard/ClipboardControls.hpp
        include/LibIO/clipboard/Windows.hpp
        include/LibIO/clipboard/Linux.hpp

        include/LibIO/mouse/MouseControls.hpp
        include/LibIO/mouse/Windows.hpp
        include/LibIO/mouse/Linux.hpp

        # Private sources
        src/keyboard/KeyboardControls.cpp
        src/keyboard/Windows.cpp
        src/keyboard/Linux.cpp

        src/mouse/MouseControls.cpp
        src/mouse/Windows.cpp
        src/mouse/Linux.cpp

        src/clipboard/Linux.cpp
        src/clipboard/ClipboardControls.cpp
        src/clipboard/Windows.cpp
)

include(X11)

# ---------------------------------------------------------
# Platform defines
# ---------------------------------------------------------
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC PLATFORM_WINDOWS)
elseif(UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(${PROJECT_NAME} PUBLIC PLATFORM_LINUX)
endif()

# ---------------------------------------------------------
# Include directories
# ---------------------------------------------------------
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---------------------------------------------------------
# Installation
# ---------------------------------------------------------

include(GNUInstallDirs)

# Install library
install(TARGETS ${PROJECT_NAME}
        EXPORT LibIOTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install public headers
install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export targets
install(EXPORT LibIOTargets
        FILE LibIOTargets.cmake
        NAMESPACE LibIO::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibIO
)

# ---------------------------------------------------------
# Package config
# ---------------------------------------------------------
include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/LibIOConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/LibIOConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibIO
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/LibIOConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibIO
)
