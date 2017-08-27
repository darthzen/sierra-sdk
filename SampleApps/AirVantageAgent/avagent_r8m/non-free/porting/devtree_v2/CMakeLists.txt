# CMakeLists for the Air Vantage Device Management Tree version 2 adapter.
cmake_minimum_required (VERSION 2.6)
project (LIBDEVTREE2)

set(DEVTREE_SPEC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LIBDEVTREE2_SRC
    ${LIBDEVTREE2_SOURCE_DIR}/devtree2_adapter.c
)

INCLUDE_DIRECTORIES(
    ${COMMON_SOURCE_DIR}
    ${EXTVARSCOMMON_SOURCE_DIR}
    ${LIBDEVTREE2_SOURCE_DIR}
    ${LIB_LINUX_SOURCE_DIR}
    ${LUA_SOURCE_DIR}/linux/liblua
    ${DEVTREE_SPEC_DIR}
)



INCLUDE_DIRECTORIES(
    # ${LIB_MIHINI_COMMON_SOURCE_DIR}:
    ${EMBEDDED_SOURCE_DIR}/libs/common
    # ${LIB_MIHINI_LUASIGNAL_SOURCE_DIR}:
    ${EMBEDDED_SOURCE_DIR}/libs/luasignal
    # ${LUA_SOURCE_DIR}:
    ${EMBEDDED_SOURCE_DIR}/luafwk/lua
    # ${LIB_EXTVARS_COMMON_SOURCE_DIR}:
    ${EMBEDDED_SOURCE_DIR}/agent/agent/devman/extvars/common            
    ${EMBEDDED_SOURCE_DIR}/agent/agent/treemgr/handlers/extvars
)

INCLUDE_DIRECTORIES(
    ${LIB_MIHINI_COMMON_SOURCE_DIR}
    ${LIB_MIHINI_LUASIGNAL_SOURCE_DIR}
    ${LUA_SOURCE_DIR}
)



add_lua_library(devtree2 DESTINATION agent/devman/extvars EXCLUDE_FROM_ALL ${LIBDEVTREE2_SRC} )
target_link_libraries(devtree2 lib_extvars)
ADD_DEPENDENCIES(devtree2 treemgr_devtree2)

ADD_CUSTOM_TARGET(devtree2_install
    # COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/defaultconfig.lua ${CMAKE_LUA_LIBRARY_OUTPUT_DIRECTORY}/agent/defaultconfig.lua
    # COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/platform.lua ${CMAKE_LUA_LIBRARY_OUTPUT_DIRECTORY}/agent/platform.lua
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/devtree2.map ${CMAKE_INSTALL_PREFIX}/resources/devtree2.map
)
ADD_DEPENDENCIES(devtree2_install devtree2)
