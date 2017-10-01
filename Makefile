sequential_debug:
	gcc src/sequential.c -O3 -D DEBUG -o out/sequential

build_debug: sequential_debug

sequential:
	gcc src/sequential.c -O3 -o out/sequential

build: sequential