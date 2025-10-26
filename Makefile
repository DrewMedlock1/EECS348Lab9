all: lab9

lab9: lab9.cpp
	g++ lab9.cpp -o lab9

clean:
	rm -f lab9