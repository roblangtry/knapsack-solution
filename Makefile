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

build_debug: sequential_debug improved_sequential_debug parralel_debug

build: sequential improved_sequential parralel

test:
	./scripts/test.sh