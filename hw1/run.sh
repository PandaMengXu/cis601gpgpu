#!/bin/bash
taskset -c 0 ./sender3 &
taskset -c 1 ./receiver3 
