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

naive_parralel_debug:
	mpicc -Wall -pedantic -O3 -fopenmp src/naive_parralel.c -D DEBUG -o out/naive_parralel

naive_parralel:
	mpicc -Wall -pedantic -O3 -fopenmp src/naive_parralel.c -o out/naive_parralel

build_debug: sequential_debug improved_sequential_debug parralel_debug naive_parralel_debug

build: sequential improved_sequential parralel naive_parralel

test:
	./scripts/test.sh