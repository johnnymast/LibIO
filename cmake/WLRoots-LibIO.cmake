find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)
find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)

set(PROTOCOL_DIR ${CMAKE_CURRENT_LIST_DIR}/../protocols)
set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

set(VK_C ${GENERATED_DIR}/virtual-keyboard-unstable-v1-protocol.c)
set(VK_H ${GENERATED_DIR}/virtual-keyboard-unstable-v1-protocol.h)

add_custom_command(
        OUTPUT ${VK_C} ${VK_H}
        COMMAND ${WAYLAND_SCANNER} private-code
        ${PROTOCOL_DIR}/virtual-keyboard-unstable-v1.xml
        ${VK_C}
        COMMAND ${WAYLAND_SCANNER} client-header
        ${PROTOCOL_DIR}/virtual-keyboard-unstable-v1.xml
        ${VK_H}
        DEPENDS ${PROTOCOL_DIR}/virtual-keyboard-unstable-v1.xml
)

set(VP_C ${GENERATED_DIR}/virtual-pointer-unstable-v1-protocol.c)
set(VP_H ${GENERATED_DIR}/virtual-pointer-unstable-v1-protocol.h)

add_custom_command(
        OUTPUT ${VP_C} ${VP_H}
        COMMAND ${WAYLAND_SCANNER} private-code
        ${PROTOCOL_DIR}/virtual-pointer-unstable-v1.xml
        ${VP_C}
        COMMAND ${WAYLAND_SCANNER} client-header
        ${PROTOCOL_DIR}/virtual-pointer-unstable-v1.xml
        ${VP_H}
        DEPENDS ${PROTOCOL_DIR}/virtual-pointer-unstable-v1.xml
)

set(DC_C ${GENERATED_DIR}/data-control-unstable-v1-protocol.c)
set(DC_H ${GENERATED_DIR}/data-control-unstable-v1-protocol.h)

add_custom_command(
        OUTPUT ${DC_C} ${DC_H}
        COMMAND ${WAYLAND_SCANNER} private-code
        ${PROTOCOL_DIR}/data-control-unstable-v1.xml
        ${DC_C}
        COMMAND ${WAYLAND_SCANNER} client-header
        ${PROTOCOL_DIR}/data-control-unstable-v1.xml
        ${DC_H}
        DEPENDS ${PROTOCOL_DIR}/data-control-unstable-v1.xml
)

target_sources(${PROJECT_NAME} PRIVATE
        ${VK_C} ${VP_C} ${DC_C}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${WAYLAND_INCLUDE_DIRS}
        ${XKBCOMMON_INCLUDE_DIRS}
        ${GENERATED_DIR}
)

target_link_libraries(${PROJECT_NAME}
        ${WAYLAND_LIBRARIES}
        ${XKBCOMMON_LIBRARIES}
)