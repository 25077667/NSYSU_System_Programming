#!/usr/bin/gnuplot

reset
set xlabel 'test cycles'
set ylabel 'time(sec)'
set title 'perfomance comparison'
set term png size 1600,1200
set output "statistic.png"


plot [:][:] './sigsuspend' using 0:1 with points title 'sigsuspend', \
            './sigwaitinfo' using 0:1 with points title 'sigwaitinfo'
