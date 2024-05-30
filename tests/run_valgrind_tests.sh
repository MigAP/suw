#!/bin/sh

VFLAGS=-s
valgrind $VFLAGS ../time -i 0 -f 10 -s 0.1 
valgrind $VFLAGS ../step -i 0 -f 1 -t 5 < time.dat
valgrind $VFLAGS ../z-tr-fc -f z_first_order_model.txt -i 0 < step.dat
valgrind $VFLAGS ../z-cl -s z_first_order_model.txt -c z_pi_controller.txt -i 0 -j 0 < step.dat
