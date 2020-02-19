cmake_minimum_required(VERSION 3.10)
project(ramen_bot)

set(CMAKE_CXX_STANDARD 17)
set(EXTERNAL_LIBRARY_FIND_PATH ext)

message(STATUS "ext path: ${LIBRARY_FIND_PATH}")

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

include_directories(src)
include_directories(${EXTERNAL_LIBRARY_FIND_PATH}/cqcppsdk/include)
include_directories(${EXTERNAL_LIBRARY_FIND_PATH})

include(${EXTERNAL_LIBRARY_FIND_PATH}/cqcppsdk/cqcppsdk.cmake)

cq_set_app_id("com.ramen.ramen_bot")

if(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
    cq_add_app(app ${SOURCE_FILES})
endif()

set(CQCPPSDK_DEV_MODE ON)
cq_add_app(ramen_bot ${SOURCE_FILES})
