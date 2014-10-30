#!/bin/bash
# This file is public domain.
(cd ../src && cmake . && make )&&
cp ../src/libHongKong.dll ./ &&

echo '#hk_sim_test#' &&
./build.sh ./hk_sim_test.cpp -o hk_sim_test&&
./hk_sim_test.exe >hk_sim_test_result.txt &&
{ read str1;read str2 ; } <hk_sim_test_result.txt
[ x$str1 = x"helloworld" -a x$str2 = x"HongKong" ] &&
 echo OK|| echo '############## BAD ################'
exit
./build.sh ./hk_SFC_test.cpp -L"../../ftdi_proprietary/amd64" -lftd2xx64  -I"../../ftdi_proprietary" -o hk_SFC_test &&
./hk_SFC_test >hk_SFC_test_result.txt 
