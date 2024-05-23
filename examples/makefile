all: step.dat \
	step.pdf \
	filt.dat  \
	filt.pdf \
	z-tf-first-order-response.dat \
	z-tf-first-order-response.pdf \
	z-tf-first-order-cl.dat \
	z-tf-first-order-cl.pdf \

step.dat:
	../time -i 0 -f 10 -s 0.01 | \
	../step -i 0 -f 2 -t 5 > step.dat

filt.dat:
	../time -i 0 -f 10 -s 0.001 | \
	../step -i 2 -f 3 -t 5 | \
	../disc-1-ord-filt -a 0.95 -i 2 > filt.dat

z-tf-first-order-response.dat: z_first_order_model.txt
	../time -i 0 -f 2 -s 0.01 | \
	../step -i 0 -f 1 -t 1 | \
	../z-tf -f z_first_order_model.txt > z-tf-first-order-response.dat

z-tf-first-order-cl.dat: z_first_order_model.txt z_pi_controller.txt
	../time -i 0 -f 3 -s 0.01 | \
	../step -i 0 -f 1 -t 1 | \
	../z-cl -s z_first_order_model.txt -c z_pi_controller.txt -i 0.0 -j 0.0 \
	> z-tf-first-order-cl.dat


step.pdf: step.dat
	gnuplot plot_step.gnu

filt.pdf:
	gnuplot plot_filt.gnu 

z-tf-first-order-response.pdf: z-tf-first-order-response.dat
	gnuplot	plot_first_order.gnu

z-tf-first-order-cl.pdf: z-tf-first-order-cl.dat
	gnuplot plot_first_order_cl.gnu

clean:
	rm *.dat *.pdf
