#!/bin/bash
taskset 0 ./sender &
taskset 1 ./receiver 
