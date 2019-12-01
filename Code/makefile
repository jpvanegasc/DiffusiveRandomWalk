DEBUG = -g -Wall -fsanitize=address -fsanitize=leak -fsanitize=undefined
all : main.o

main.o : main.cpp
	g++ -O2 $< -o $@
	time ./$@

.PHONY : parallel
parallel : main.cpp
	g++ -fopenmp -O2 $<
	time ./a.out

.PHONY : debug
debug : main.cpp
	g++ $(DEBUG) $<
	./a.out

.PHONY : valgrind
valgrind : main.cpp
	g++ -g $<
	valgrind --track-origins=yes --leak-check=full ./a.out

.PHONY : cachegrind
cachegrind : main.cpp
	g++ -g $<
	valgrind --tool=cachegrind ./a.out

.PHONY : gprof
gprof : main.cpp
	g++ -Wall -pg $<
	./a.out
	gprof a.out gmon.out > analysis.txt
	most analysis.txt

.PHONY : perf
perf : main.cpp
	g++ -Wall -pg $<
	perf record ./a.out ; perf report

.PHONY : clean
clean :
	rm -f *.o *.txt *.out* debug main *.data

datos.txt : sumas.x
	time ./$< > $@