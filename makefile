APPNAME=game_d16
SOURCES=main.cpp d16.cpp  minigame_crafting.cpp   minigame_combat.cpp
HEADERS=d16.h  minigame_crafting.h  minigame_combat.h
$(APPNAME): main.cpp d16.cpp d16.h minigame_crafting.cpp minigame_crafting.h  minigame_combat.cpp minigame_combat.h
	gcc -o $(APPNAME) -Wall -Wextra $(SOURCES)

run: $(APPNAME)
	./$(APPNAME)
