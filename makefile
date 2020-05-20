APPNAME=game_d16
SOURCES=main.cpp d16.cpp  minigame/crafting/crafting.cpp   minigame/combat/combat.cpp interpreter.cpp minigame/interface.cpp
HEADERS=d16.h  minigame/crafting/crafting.h  minigame/combat/combat.h interpreter.h minigame/interface.hpp
$(APPNAME): $(SOURCES) $(HEADERS)
	g++ -o $(APPNAME) -Wall -Wextra $(SOURCES)

testbuild:  tags $(SOURCES) $(HEADERS)
	g++ -o /dev/null -Wall -Wextra -Wfatal-errors $(SOURCES)


run: $(APPNAME)
	./$(APPNAME)

tags: $(SOURCES) $(HEADERS)
	ctags-exuberant -R *

.phony: nofile
