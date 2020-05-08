game_d16: main.cpp d16.cpp d16.h
	gcc -Wall -Wextra main.cpp d16.cpp -o game_d16

run: game_d16
	./game_d16
