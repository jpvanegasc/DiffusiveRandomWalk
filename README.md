# Random Walk, Entropy And the Arrow of Time 
Repository for the final project of the Herramientas Computacionales course, fall semester 
of 2019. This project deals with (as the title states) Entropy on a diffusive system, 
represented here using a random walk lattice model.

## Authors
* Juan Pablo Vanegas Correa
* Juan Bernardo Benavides

## Compilation 
All code must be compiled using c++11. A standard compilation would be 
```bash
g++ -std=c++11 program_name_here.cpp
```
Also some of the code is parallelized using openmp, so in order to use this feature you 
would need to include an extra flag, like this:
```bash
g++ -std=c++11 -fopenmp program_name_here.cpp
```
However, makefiles are included to save time and effort in this matter for some files, 
and diferent debugging, profiling and optimization checks are also automatized. Please do 
check the following table:

### Implemented Automatization
| Shell Command   | What it does oversimplified                                               |
|-----------------|---------------------------------------------------------------------------|
| make            | Compiles using O2 optimization, c++11 standard and runs program using time|
| make parallel   | Same as make, but compiles with -fopenmp flag                             |
| make debug      | Compiles with all warning flags, saves debug info using -g flag and runs  |
| make valgrind   | Compiles with -g flag and runs using valgrind                             |
| make cachegrind | Same as valgrind but using runs using cachegrind                          |
| make gprof      | Compiles with -pg flag, runs and display gprof info                       |
| make perf       | Same as gprof, but runs and displays perf info                            |

