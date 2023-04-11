output: main.o
	g++ main.o -o run.out

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o run.out