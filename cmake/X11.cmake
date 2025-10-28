if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT WAYLAND)
    find_package(X11 REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE X11 Xrandr Xtst)
endif()
