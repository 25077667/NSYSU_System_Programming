#!/usr/bin/gnuplot

reset
set xlabel 'test cycles'
set ylabel 'time(sec)'
set title 'perfomance comparison (p-value=0.0 underflow)'
set term png size 1200,900
set output "statistic.png"


plot [:][:] './sigsuspend' using 0:1 with points title 'sigsuspend', \
            './sigwaitinfo' using 0:1 with points title 'sigwaitinfo'