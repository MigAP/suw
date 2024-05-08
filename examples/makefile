all: step.dat filt.dat z-tr-fc.dat z-cl.dat\
     step.pdf filt.pdf 

step.dat:
	../time -i 0 -f 10 -s 1 | ../step -i 3 -f 42 -t 5 > step.dat

filt.dat:
	../time -i 0 -f 10 -s 0.001 | ../step -i 3 -f 42 -t 5 | ../disc-1-ord-filt -a 0.95 -i 3 > filt.dat

z-tr-fc.dat:
	../time -i 0 -f 10 -s 0.001 | ../step -i 3 -f 42 -t 5 | ../z-tr-fc -f z_fct.txt > z-tr-fc.dat

z-cl.dat:
	../time -i 0 -f 10 -s 0.001 | \
	../step -i 3 -f 42 -t 5 | \
	../z-cl -s s_tf.txt -c c_tf.txt -i 1.0 -j 2.0 > z-cl.dat

step.pdf: step.dat
	gnuplot plot.gnu

filt.pdf:
	gnuplot plot_filt.gnu 

clear:
	rm step.dat step.pdf filt.dat filt.pdf z-cl.dat
