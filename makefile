APPNAME=game_d16
SOURCES=main.cpp d16.cpp  minigame/crafting/crafting.cpp   minigame/combat/combat.cpp
HEADERS=d16.h  minigame/crafting/crafting.h  minigame/combat/combat.h
$(APPNAME): $(SOURCES) $(HEADERS)
	g++ -o $(APPNAME) -Wall -Wextra $(SOURCES)

run: $(APPNAME)
	./$(APPNAME)
