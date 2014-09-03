#!/bin/bash
#libHongKong.dll
g++ "$@" libhongkong.cpp SFC.cpp ch.cpp lowlevel_io.cpp fastio.cpp -Wall -std=c++0x -L"../ftdi_proprietary/amd64" -lftd2xx64  -I"../ftdi_proprietary" -shared -o libHongKong.dll

#testcode
#g++ -std=c++0x test/testmain.cpp  -I. -L. -lHongKong
