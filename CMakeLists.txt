
cmake_minimum_required(VERSION 2.4)
project(tdb)

set(CMAKE_CXX_STANDARD 11)

# Find required glog package
find_package(GLOG REQUIRED)
if(GLOG_FOUND)
    message(STATUS "glog library found")
else()
    message(FATAL_ERROR "glog library is needed but can not be found")
endif()

# Find required gflags package
find_package(GFLAGS REQUIRED)
if(GFLAGS_FOUND)
    message(STATUS "gflags library found")
else()
    message(FATAL_ERROR "gflags library is needed but cant be found")
endif()

file(GLOB SRC
  src/*.cc
)

include_directories(${PROJECT_SOURCE_DIR}/src) 


add_executable(tdb ${SRC})

target_link_libraries(tdb
-lglog
-lgflags
-lpthread
)
