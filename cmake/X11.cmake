if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XTST REQUIRED xtst)

    pkg_check_modules(XRANDR REQUIRED xrandr)


    target_include_directories(LibIO
            PUBLIC
            ${XTST_INCLUDE_DIRS}
    )

    target_link_directories(LibIO
            PUBLIC
            ${XTST_LIBRARY_DIRS}
    )

    target_link_libraries(LibIO
            PUBLIC
            X11::X11
            X11::Xrandr
            ${XTST_LIBRARIES}
    )
endif()
