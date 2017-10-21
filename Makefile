sequential_debug:
	mpicc -Wall -pedantic -O3 -fopenmp src/sequential.c -D DEBUG -o out/sequential

sequential:
	mpicc -Wall -pedantic -O3 -fopenmp src/sequential.c -o out/sequential

improved_sequential_debug:
	mpicc -Wall -pedantic -O3 -fopenmp src/improved-sequential.c -D DEBUG -o out/improved-sequential

improved_sequential:
	mpicc -Wall -pedantic -O3 -fopenmp src/improved-sequential.c -o out/improved-sequential

parralel_debug:
	mpicc -Wall -pedantic -O3 -fopenmp src/parralel.c -D DEBUG -o out/parralel

parralel:
	mpicc -Wall -pedantic -O3 -fopenmp src/parralel.c -o out/parralel

better_parralel_debug:
	mpicc -Wall -pedantic -O3 -fopenmp src/better_parralel.c -D DEBUG -o out/better_parralel

better_parralel:
	mpicc -Wall -pedantic -O3 -fopenmp src/better_parralel.c -o out/better_parralel

naive_parralel_debug:
	mpicc -Wall -pedantic -O3 -fopenmp src/naive_parralel.c -D DEBUG -o out/naive_parralel

naive_parralel:
	mpicc -Wall -pedantic -O3 -fopenmp src/naive_parralel.c -o out/naive_parralel

heuristic_debug:
	mpicc -Wall -pedantic -O3 -fopenmp src/heuristic.c -D DEBUG -o out/heuristic

heuristic:
	mpicc -Wall -pedantic -O3 -fopenmp src/heuristic.c -o out/heuristic

build_debug: sequential_debug improved_sequential_debug parralel_debug naive_parralel_debug better_parralel_debug heuristic_debug

build: sequential improved_sequential parralel naive_parralel better_parralel heuristic

test:
	./scripts/test.sh