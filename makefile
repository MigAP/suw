all: time step disc-1-ord-filt z-tf z-cl \

CC=gcc
# CFLAGS=-Wall -Wextra -fanalyzer -g -pedantic-errors -std=c99 -D_POSIX_C_SOURCE=200809L
CFLAGS=-Wall -Wextra -fanalyzer -pedantic-errors -std=c99 -D_POSIX_C_SOURCE=200809L -O2
LIBS= -lm
DEPS= util.h z-utils.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

time: time.o util.o
	$(CC) -o time time.o util.o

step: step.o util.o
	$(CC) -o step step.o util.o

disc-1-ord-filt: disc-1-ord-filt.o util.o
	$(CC) -o disc-1-ord-filt disc-1-ord-filt.o util.o

z-tf: z-tf.o util.o z-utils.o
	$(CC) -o z-tf z-tf.o util.o z-utils.o

z-cl: z-cl.o util.o z-utils.o
	$(CC) -o z-cl z-cl.o util.o z-utils.o

clean:
	rm *.o step time disc-1-ord-filt z-tf z-cl
