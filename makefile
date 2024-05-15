all: time step disc-1-ord-filt z-tr-fc z-cl tests \

CC=gcc
CFLAGS=-Wall -Wextra -fanalyzer -g -pedantic-errors -std=c99
# CFLAGS=-Wall -g -pedantic-errors 
LIBS= -lm
DEPS= util.h z-tf.h

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

tests: tests.o
	$(CC) -o tests tests.o util.o z-tf.o

clear:
	rm *.o step time z-tr-fc z-cl
