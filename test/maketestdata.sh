#!/bin/bash
# This file is public domain.
dd if=/dev/zero bs=2097152 count=1>testdata.txt
dd if=<(echo -n helloworld) of=testdata.txt bs=1 seek=0  conv=notrunc 
dd if=<(echo -n HongKong) of=testdata.txt bs=1 seek=1000000  conv=notrunc 
