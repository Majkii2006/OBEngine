build: main.cpp 
	g++ main.cpp --std=c++17 -g -Wall -o main

clean:
	rm -f main

run: 
	./main
