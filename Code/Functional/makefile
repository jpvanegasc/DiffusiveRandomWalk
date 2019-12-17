DEBUG = -g -Wall -fsanitize=address -fsanitize=leak -fsanitize=undefined
C_STD = -std=c++11
all : coffee.o

coffee.o : coffee.cpp
	g++ $(C_STD) -O2 $< -o $@
	time ./$@ > coffee.txt

.PHONY : parallel
parallel : coffee.cpp
	g++ $(C_STD) -fopenmp -O2 $<
	time ./a.out

.PHONY : debug
debug : coffee.cpp
	g++ $(C_STD) $(DEBUG) $<
	./a.out

.PHONY : valgrind
valgrind : coffee.cpp
	g++ $(C_STD) -g $<
	valgrind --track-origins=yes --leak-check=full ./a.out

.PHONY : cachegrind
cachegrind : coffee.cpp
	g++ $(C_STD) -g $<
	valgrind --tool=cachegrind ./a.out

.PHONY : gprof
gprof : coffee.cpp
	g++ $(C_STD) -Wall -pg $<
	./a.out
	gprof a.out gmon.out > analysis.txt
	most analysis.txt

.PHONY : perf
perf : coffee.cpp
	g++ $(C_STD) -Wall -pg $<
	perf record ./a.out ; perf report

.PHONY : clean
clean :
	rm -f *.o *.txt *.out* debug coffee *.data

.PHONY : plot
plot : coffee.txt
	gnuplot plot.gp