all: tests 

CC=gcc
CFLAGS=-Wall -Wextra -fanalyzer -g -pedantic-errors -std=c99 -D_POSIX_C_SOURCE=200809L
# CFLAGS=-Wall -Wextra -fanalyzer -pedantic-errors -std=c99 -D_POSIX_C_SOURCE=200809L -O2
LIBS= -lm
DEPS= ../util.h ../z-utils.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tests: tests.o ../util.o ../z-utils.o
	$(CC) -o tests tests.o ../util.o ../z-utils.o

all_tests: 
	./tests_input_output.sh && ./tests
clean:
	rm *.o tests
