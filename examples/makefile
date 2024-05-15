all: step.dat filt.dat z-tr-fc.dat z-cl.dat z-tf-first-order-response.dat\
     step.pdf filt.pdf z-cl.pdf z-tf-first-order-response.pdf

step.dat:
	../time -i 0 -f 10 -s 0.01 | ../step -i 0 -f 2 -t 5 > step.dat

filt.dat:
	../time -i 0 -f 10 -s 0.001 | ../step -i 2 -f 3 -t 5 | ../disc-1-ord-filt -a 0.95 -i 2 > filt.dat

z-tr-fc.dat:
	../time -i 0 -f 10 -s 0.001 | ../step -i 0 -f 2 -t 5 | ../z-tr-fc -f z_fct.txt > z-tr-fc.dat

z-cl.dat:
	../time -i 0 -f 1 -s 0.001 | \
	../step -i 0 -f 1 -t 0.5 | \
	../z-cl -s s_tf.txt -c c_tf.txt -i 0.0 -j 0.0 > z-cl.dat

z-tf-first-order-response.dat: z_first_order_model.txt
	../time -i 0 -f 2 -s 0.01 | \
	../step -i 0 -f 1 -t 1 | \
	../z-tr-fc -f z_first_order_model.txt > z-tf-first-order-response.dat


step.pdf: step.dat
	gnuplot plot_step.gnu

filt.pdf:
	gnuplot plot_filt.gnu 

z-cl.pdf: z-cl.dat
	gnuplot plot_cl.gnu

z-tf-first-order-response.pdf: z-tf-first-order-response.dat
	gnuplot	plot_z_first_order.gnu

clear:
	rm *.dat *.pdf
