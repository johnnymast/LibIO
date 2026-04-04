if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)
    find_package(Xrandr REQUIRED)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XTST REQUIRED xtst)

    target_include_directories(${PROJECT_NAME} PUBLIC ${XTST_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${XTST_LIBRARIES})

    target_link_libraries(${PROJECT_NAME} PUBLIC X11 Xrandr ${XTST_LIBRARIES})
endif()

