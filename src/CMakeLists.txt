cmake_minimum_required (VERSION 2.8)

project (libHongKong CXX)

#g++ "$@" libhongkong.cpp SFC.cpp ch.cpp lowlevel_io.cpp fastio.cpp -Wall -std=c++0x -L"../ftdi_proprietary/amd64" -lftd2xx64  -I"../ftdi_proprietary" -shared -o libHongKong.dll
link_directories(${CMAKE_CURRENT_SOURCE_DIR}"../ftdi_proprietary/amd64")
include_directories("../ftdi_proprietary")
add_library (libHongKong libhongkong.cpp SFC.cpp ch.cpp lowlevel_io.cpp fastio.cpp  )
add_definitions(-std=c++11 -Wall)
target_link_libraries(libHongKong ftd2xx64)
