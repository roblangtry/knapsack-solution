sequential_debug:
	mpicc -Wall -pedantic -O3 -fopenmp src/sequential.c -D DEBUG -o out/sequential

sequential:
	mpicc -Wall -pedantic -O3 -fopenmp src/sequential.c -o out/sequential

parralel_debug:
	mpicc -Wall -pedantic -O3 -fopenmp src/parralel.c -D DEBUG -o out/parralel

parralel:
	mpicc -Wall -pedantic -O3 -fopenmp src/parralel.c -o out/parralel

build_debug: sequential_debug parralel_debug

build: sequential parralel

test:
	./scripts/test.sh