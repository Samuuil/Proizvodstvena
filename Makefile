all:main.exe
vector.o: vector.h vector.c
	gcc -c vector.c -o vector.o
xor.o: xor.h xor.c
	gcc -c xor.c -o xor.o
main.exe: main.c vector.o xor.o
	gcc main.c vector.o  xor.o -o main.exe
clean:
	del *.o *.exe