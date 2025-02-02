compile:
	gcc src/*.c -o build/qcc

run:
	build/qcc test/in.q test/out.c
	# gcc test/out.c -o test/out

cc:
	gcc test/out.c -o test/out

qqcc:
	gcc src/*.c -o build/qcc
	build/qcc test/in.q test/out.c
	gcc test/out.c -o test/out