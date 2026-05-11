cmake_minimum_required(VERSION 3.16)
project(LibIO VERSION 1.0.0 LANGUAGES C CXX)

set(PROTOCOL_DIR "${CMAKE_SOURCE_DIR}/protocols")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT (WINDOWS_PLATFORM OR X11_PLATFORM OR WLROOTS_PLATFORM))
    message(FATAL_ERROR]
            "No backend selected. Enable at least one of: "
            "WINDOWS_PLATFORM, X11_PLATFORM, WLROOTS_PLATFORM."
    )
endif ()

if (NOT (WINDOWS_CLIPBOARD OR X11_CLIPBOARD OR WLROOTS_CLIPBOARD))
    message(FATAL_ERROR
            "No clipboard backend selected. Enable at least one of the following: "
            "WINDOWS_CLIPBOARD, X11_CLIPBOARD, WLROOTS_CLIPBOARD."
    )
endif ()

if (NOT (WINDOWS_KEYBOARD OR X11_KEYBOARD OR WLROOTS_KEYBOARD))
    message(FATAL_ERROR
            "No keyboard backend selected. Enable at least one of the following: "
            "WINDOWS_KEYBOARD, X11_KEYBOARD, WLROOTS_KEYBOARD."
    )
endif ()


if (NOT (WINDOWS_MOUSE OR X11_MOUSE OR WLROOTS_MOUSE))
    message(FATAL_ERROR
            "No mouse backend selected. Enable at least one of the following: "
            "WINDOWS_MOUSE, X11_MOUSE, WLROOTS_MOUSE."
    )
endif ()


# ---------------------------------------------------------
# Library
# ---------------------------------------------------------
add_library(${PROJECT_NAME} SHARED
        include/private/LibIO/MainThreadExecutor.hpp

        src/MouseControls.cpp
        src/ClipboardControls.cpp
        src/KeyboardControls.cpp
        src/LibIO.cpp

)

if (WINDOWS_PLATFORM)
    target_compile_definitions(${PROJECT_NAME} PUBLIC PLATFORM_WINDOWS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WINDOWS_CLIPBOARD)
    target_sources(${PROJECT_NAME} PRIVATE
            src/clipboard/backends/Win32.cpp
            src/keyboard/backends/Win32.cpp
            src/mouse/backends/Win32.cpp
    )
endif ()

if (X11_PLATFORM)
    target_compile_definitions(${PROJECT_NAME} PUBLIC PLATFORM_X11)
    target_sources(${PROJECT_NAME} PRIVATE
            src/keyboard/backends/X11.hpp
            src/keyboard/backends/X11.cpp

            src/mouse/backends/X11.hpp
            src/mouse/backends/X11.cpp
    )
    include(X11-LibIO)
endif ()

if (WLROOTS_PLATFORM)
    target_compile_definitions(${PROJECT_NAME} PUBLIC PLATFORM_WLROOTS)
    target_sources(${PROJECT_NAME} PRIVATE

            src/mouse/backends/WLRoots.cpp
    )
    include(WLRoots-LibIO)
endif ()


if (X11_CLIPBOARD)
    target_compile_definitions(${PROJECT_NAME} PUBLIC X11_CLIPBOARD)
    target_sources(${PROJECT_NAME} PRIVATE
            src/clipboard/backends/X11.hpp
            src/clipboard/backends/X11.cpp
    )
    include(X11-LibIO)
endif ()

if (WLROOTS_KEYBOARD)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WLROOTS_KEYBOARD)
    target_sources(${PROJECT_NAME} PRIVATE
            src/keyboard/backends/WLRoots.hpp
            src/keyboard/backends/WLRoots.cpp
    )
endif ()


if (WLROOTS_MOUSE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WLROOTS_MOUSE)
    target_sources(${PROJECT_NAME} PRIVATE
            src/mouse/backends/WLRoots.hpp
            src/mouse/backends/WLRoots.cpp
    )
endif ()

if (WLROOTS_CLIPBOARD)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WLROOTS_CLIPBOARD)
    target_sources(${PROJECT_NAME} PRIVATE
            src/clipboard/backends/WLRoots.hpp
            src/clipboard/backends/WLRoots.cpp
    )
endif ()


if (SPARKLE_SUPERBUILD)
    message(STATUS "❤️  ${PROJECT_NAME} USING Superbuild")
endif ()

if (X11_PLATFORM OR WLROOTS_PLATFORM)
    target_compile_definitions(${PROJECT_NAME} PUBLIC PLATFORM_LINUX)
endif ()


target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/private
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_compile_definitions(${PROJECT_NAME} PRIVATE LIBIO_EXPORTS)

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
