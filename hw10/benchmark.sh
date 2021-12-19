#!/bin/bash

make clean dep all

R=(500 800 1000)
C=(10 100 1000)

for r in ${R[@]}; do
    for c in ${C[@]}; do
        rate=$(./main.elf 1000 $r $c 3)
        echo "$r $c ${rate}" >>result
    done
done

B=$(seq 10)

for b in ${B[@]}; do
    for c in ${C[@]}; do
        rate=$(./main.elf 1000 500 $c $b)
        echo "$b $c ${rate}" >>result
    done
done
