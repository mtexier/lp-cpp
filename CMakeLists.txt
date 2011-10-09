project ( lp2 )

cmake_minimum_required ( VERSION 2.6 )

include_directories ( ${PROJECT_SOURCE_DIR}/include )

set ( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH} )
include ( lp.boost )

set ( CMAKE_CXX_FLAFS         "-Wall -Wfatal-errors" )
set ( CMAKE_CXX_FLAFS_RELEASE "-O3 -DNDEBUG" )
set ( CMAKE_CXX_FLAFS_DEBUG   "-g -O0 -DDEBUG" )

add_executable ( lp.x
  src/main.cpp
)
