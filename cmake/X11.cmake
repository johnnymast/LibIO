if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)
    find_package(Xrandr REQUIRED)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XTST REQUIRED xtst)

    target_include_directories(${PROJECT_NAME} PRIVATE
            ${XTST_INCLUDE_DIRS}
    )

    target_link_directories(${PROJECT_NAME} PRIVATE
            ${XTST_LIBRARY_DIRS}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
            X11::X11
            X11::Xrandr
            ${XTST_LIBRARIES}
    )
endif()
