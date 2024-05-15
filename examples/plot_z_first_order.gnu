set terminal pdf
set output "z-tf-first-order-response.pdf"
set datafile separator "\t"
set title "Step and System output"
set xlabel "Time [s]"
set grid

plot "z-tf-first-order-response.dat" \
  using 1:2 with lines title "step","" \
  using 1:3 with lines title "output"