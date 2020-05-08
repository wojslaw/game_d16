game_d16: main.cpp d16.cpp d16.h minigame.cpp minigame.h
	gcc -Wall -Wextra d16.cpp minigame.cpp main.cpp  -o game_d16

run: game_d16
	./game_d16
