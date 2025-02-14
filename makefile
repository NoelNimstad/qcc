compile:
	gcc src/*.c -o build/qlang

run:
	build/qlang test/input.q test/out