#!/bin/bash
rm $3
for((i=0; i<5; i+=1));do
    ./cpu_latency_boxplot_cpuindex $1 $2 $i >> $3 &
    echo $3
    echo "cpu index $i"
done
