set terminal pdf
set output "step.pdf"
set datafile separator "\t"
set title "Step output"
set xlabel "Time [s]"
set ylabel "Step output"
set grid

plot "step.dat" using 1:2 with lines