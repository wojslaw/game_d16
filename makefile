d16: main.cpp d16.cpp d16.h
	gcc -Wall -Wextra main.cpp d16.cpp -o d16

run: d16
	./d16
