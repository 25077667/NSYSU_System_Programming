#!/bin/bash

## Origin sigsuspend version
make all
for i in $(seq 2000); do
    e=$({ time ./target 65536; } 2>&1)
    echo -n "$e" | tr '\n' ' ' | awk -F" " '{print $6}' | sed "s/..\(.*\)s$/\1/" | xargs echo >>sigsuspend
    echo sigsuspend $i
done

## Hooked sigwaitinfo version
make hook
for i in $(seq 2000); do
    e=$({ time ./target 65536; } 2>&1)
    echo -n "$e" | tr '\n' ' ' | awk -F" " '{print $6}' | sed "s/..\(.*\)s$/\1/" | xargs echo >>sigwaitinfo
    echo sigwaitinfo $i
done

## plot the graph
gnuplot plot.gp
