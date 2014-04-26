#!/bin/bash
for((i=0; i<5; i+=1));do
    ./cpu_latency_boxplot_cpuindex $1 $2 $i &
    echo "cpu index $i"
done
