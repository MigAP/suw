set terminal pdf
set output "z-tf-first-order-cl.pdf"
set datafile separator "\t"
set title "Closed loop plot"
set xlabel "Time [s]"
set grid

plot "z-tf-first-order-cl.dat" using 1:2 with lines title "reference", \
     "" using 1:3 with lines title "error", \
     "" using 1:4 with lines title "control", \
     "" using 1:5 with lines title "output", \