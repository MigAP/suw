set terminal pdf
set output "filt.pdf"
set datafile separator "\t"
set title "Step and filter output"
set xlabel "Time [s]"
set grid

plot "filt.dat" using 1:2 with lines title "step","" using 1:3 with lines title "filter"