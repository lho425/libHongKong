#libHongKong.dll
g++ "$@"  -Wall -std=c++11 -L./  -lHongKong  -I../src -I"../ftdi_proprietary"
