all: time step disc-1-ord-filt z-tr-fc z-cl \
	step.dat filt.dat z-tr-fc.dat \
	step.pdf filt.pdf 

CC=clang
CFLAGS=-Wall -g -pedantic-errors -std=c99
# CFLAGS=-Wall -g -pedantic-errors 
LIBS= -lm
DEPS= util.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

time: time.o util.o
	$(CC) -o time time.o util.o

step: step.o util.o
	$(CC) -o step step.o util.o

disc-1-ord-filt: disc-1-ord-filt.o util.o
	$(CC) -o disc-1-ord-filt disc-1-ord-filt.o util.o

z-tr-fc: z-tr-fc.o util.o z-tf.o
	$(CC) -o z-tr-fc z-tr-fc.o util.o z-tf.o

z-cl: z-cl.o util.o z-tf.o
	$(CC) -o z-cl z-cl.o util.o z-tf.o

step.dat:
	./time -i 0 -f 10 -s 1 | ./step -i 3 -f 42 -t 5 > step.dat

filt.dat:
	./time -i 0 -f 10 -s 0.001 | ./step -i 3 -f 42 -t 5 | ./disc-1-ord-filt -a 0.95 -i 3 > filt.dat

z-tr-fc.dat:
	./time -i 0 -f 10 -s 0.001 | ./step -i 3 -f 42 -t 5 | ./z-tr-fc -f z_fct.txt > z-tr-fc.dat

step.pdf: step.dat
	gnuplot plot.gnu

filt.pdf:
	gnuplot plot_filt.gnu 

clear:
	rm *.o step time step.dat step.pdf filt.dat filt.pdf \
	z-tr-fc
