all: ./obj
	g++ *.o -std=c++0x -D__LINUX_COMPILE -o bin/ClosestPoint.out
obj:
	g++ -c src/* -I ./include/  -w -lboost_unit_test_framework
clean:
	rm *.o bin/*