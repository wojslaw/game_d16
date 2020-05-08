APPNAME=game_d16
$(APPNAME): main.cpp d16.cpp d16.h minigame.cpp minigame.h  minigame_combat.cpp minigame_combat.h
	gcc -o $(APPNAME) -Wall -Wextra d16.cpp minigame.cpp main.cpp minigame_combat.cpp

run: $(APPNAME)
	./$(APPNAME)
