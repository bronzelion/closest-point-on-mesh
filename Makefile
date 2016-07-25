all: ./main.o
	g++ *.o -std=c++0x -D__LINUX_COMPILE -o ClosestPoint.out
main.o:
	g++ -c src/* -I ./include/  -w 
clean:
	rm *.o 
