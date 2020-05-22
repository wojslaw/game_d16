APPNAME=game_d16
SOURCES=\
		main.cpp \
		d16.cpp \
		minigame/crafting/crafting.cpp \
		minigame/combat/combat.cpp \
		minigame/interface.cpp \
		minigame/combat/rollmod.cpp \
		minigame/combat/stattype.cpp
HEADERS= \
		 d16.h \
		 minigame/interface.hpp \
		 minigame/crafting/crafting.h \
		 minigame/combat/combat.h \
		 minigame/combat/rollmod.hpp \
		 minigame/combat/stattype.hpp
$(APPNAME): $(SOURCES) $(HEADERS)
	g++ -o $(APPNAME) -Wall -Wextra $(SOURCES)

testbuild:  tags $(SOURCES) $(HEADERS)
	g++ -o /dev/null -Wall -Wextra -Wfatal-errors $(SOURCES)


run: $(APPNAME)
	./$(APPNAME)

tags: $(SOURCES) $(HEADERS)
	ctags-exuberant -R *

.phony: nofile
