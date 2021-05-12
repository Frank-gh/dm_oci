cmake_minimum_required (VERSION 3.11)
project (dmoci)

set(CMAKE_CXX_FLAGS "-Wall")
# set(CMAKE_CXX_FLAGS "-Wall -DOTL_ODBC_UNIX -D__GNUC__")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_BUILD_TYPE Debug)

include_directories(./)
include_directories(/usr/local/include)
include_directories(/home/frank/dmdbms/drivers/oci/include)

add_executable(dmoci main.cpp)
# target_link_libraries(dmoci /home/frank/dmdbms/drivers/oci/libdmoci.so)
target_link_libraries(dmoci /home/frank/dmdbms/bin/libdodbc.so)