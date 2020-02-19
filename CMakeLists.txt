cmake_minimum_required(VERSION 3.10)
project(ramen_bot)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/ext)

message(STATUS "src path: ${SOURCE_PATH}")
message(STATUS "ext path: ${EXTERNAL_LIBRARY_PATH}")

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_PATH}/*.cpp)

include_directories(${SOURCE_PATH})
include_directories(${EXTERNAL_LIBRARY_PATH}/cqcppsdk/include)
include_directories(${EXTERNAL_LIBRARY_PATH})

include(${EXTERNAL_LIBRARY_PATH}/cqcppsdk/cqcppsdk.cmake)

cq_set_app_id("com.ramen.ramen_bot")

if(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
    cq_add_app(app ${SOURCE_FILES})
endif()

set(CQCPPSDK_DEV_MODE ON)
cq_add_app(ramen_bot ${SOURCE_FILES})
