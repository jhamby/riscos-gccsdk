# Example makefile to demonstrate the use of mkresfs

example: main.o module.o resfs.o
	gcc -mmodule -mlibscl -o example main.o module.o resfs.o

main.o: module.h resfs.h
	gcc -mmodule -mlibscl -c -O3 main.c

module.o module.h: module.cmhg
	cmunge -o module.o -d module.h -s module.s -tgcc -32bit module.cmhg

resfs.o resfs.h:
	mkresfs -o resfs.c -p resfs.h -v TestData
	gcc -mmodule -mlibscl -c -O3 resfs.c
