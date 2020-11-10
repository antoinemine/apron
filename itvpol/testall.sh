#!/bin/bash

make all || exit

mv log log.old

echo "Test started `date`"| tee log

for i in  Il Ill MPZ Ri Rll MPQ D Dl
do
    echo "Test for $i `date`" | tee -a log
    ./fpptest$i +120 2>&1 | tee -a  log
done

echo "Test finished at `date`"| tee -a log

grep error log
