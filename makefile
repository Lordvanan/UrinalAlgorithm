Urinal: main.o
	g++ main.o -o Urinal
	rm *.o

main.o: main.cpp
	g++ -c main.cpp -o main.o
