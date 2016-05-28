#!/bin/bash
# diffs should run clean
make
./run -s 0 test/d2/in1.bin test/d2/in2.bin
diff out5.txt test/d2/out5.txt
./run -s 0 test/d3/in1.bin test/d3/in2.bin
diff out5.txt test/d3/out5.txt
./run -s 0 test/d4/in1.bin test/d4/in2.bin
diff out5.txt test/d4/out5.txt
rm out5.txt
