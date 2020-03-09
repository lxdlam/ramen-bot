cmake_minimum_required(VERSION 3.10)
project(ramen_bot)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/ext)

set(CXX_COMPILER_FLAGS "-fmax-errors=5")

message(STATUS "src path: ${SOURCE_PATH}")
message(STATUS "ext path: ${EXTERNAL_LIBRARY_PATH}")

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_PATH}/*.cpp)

include_directories(${SOURCE_PATH})
include_directories(${EXTERNAL_LIBRARY_PATH}/cqcppsdk/include)
include_directories(${EXTERNAL_LIBRARY_PATH}/toml11)

set(CROSSGUID_TESTS OFF)
add_subdirectory(${EXTERNAL_LIBRARY_PATH}/crossguid)
include_directories(${EXTERNAL_LIBRARY_PATH}/crossguid/include)

add_subdirectory(${EXTERNAL_LIBRARY_PATH}/fmt EXCLUDE_FROM_ALL)
include_directories(fmt::fmt-header-only)

include(${EXTERNAL_LIBRARY_PATH}/cqcppsdk/cqcppsdk.cmake)

cq_set_app_id("com.ramen.ramen_bot")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILER_FLAGS}")

if(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
    add_compile_definitions(RELEASE)
    cq_add_app(ramen_bot ${SOURCE_FILES})
    target_link_libraries(ramen_bot PRIVATE fmt::fmt-header-only crossguid)
endif()

set(CQCPPSDK_DEV_MODE ON)
cq_add_app(ramen_bot_dev ${SOURCE_FILES})
target_link_libraries(ramen_bot_dev PRIVATE fmt::fmt-header-only crossguid)
