compile:
	gcc src/*.c -o build/qcc

run:
	build/qcc test/in.q test/out.c