cmake_minimum_required(VERSION 3.10)
project(ramen_bot)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/ext)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CXX_COMPILER_FLAGS "-ferror-limit=3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CXX_COMPILER_FLAGS "-fmax-errors=3")
endif()

message(STATUS "src path: ${SOURCE_PATH}")
message(STATUS "ext path: ${EXTERNAL_LIBRARY_PATH}")

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_PATH}/*.cpp)

include_directories(${SOURCE_PATH})
include_directories(${EXTERNAL_LIBRARY_PATH}/cqcppsdk/include)
include_directories(${EXTERNAL_LIBRARY_PATH}/toml11)

set(CROSSGUID_TESTS OFF CACHE BOOL "Disable crossguid tests")
add_subdirectory(${EXTERNAL_LIBRARY_PATH}/crossguid)
include_directories(${EXTERNAL_LIBRARY_PATH}/crossguid/include)

add_subdirectory(${EXTERNAL_LIBRARY_PATH}/fmt EXCLUDE_FROM_ALL)
include_directories(fmt::fmt-header-only)

include(${EXTERNAL_LIBRARY_PATH}/cqcppsdk/cqcppsdk.cmake)

cq_set_app_id("com.ramen.ramen_bot")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILER_FLAGS}")

link_libraries(fmt::fmt)
link_libraries(crossguid)

if(CQ_CAN_BUILD_STD_MODE)
    add_compile_definitions(RELEASE)
    cq_add_std_app(${SOURCE_FILES})
endif()

cq_add_dev_app(${SOURCE_FILES})
