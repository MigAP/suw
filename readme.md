# Simulink the Unix Way 

This repository gathers different executables to run simple
simulations involving discrete transfert functions. The project is
heavily inspired from [](https://adamsgaard.dk/unix-for-science.html).

Each executable is the equivalent of a *block* in Simulink. A
simulation consist of a shell script or a makefile calling different
executables. Each *block* has its corresponding `manpage`, e.g., run
`man step.1` to read the documentation of `step`.

You can find more information about the project in:
[](https://www.mig-ap.com)

Current implemented *blocks*:
- time
- step
- filt-1ord
- z-tf
- z-cl

## Examples

To run the simulation examples run `make` in the `examples` directory.

## Tests

Different tests are available: 

1. The executable `tests` to test the "low level" functionalities of
   the code.
2. The shell script `tests_input_output.sh` to test the input and
   output of the blocks individually.
3. Different tests using [](https://valgrind.org/) are grouped
   together in a shell script `valgrind_tests/run_valgrind_tests.sh`.
