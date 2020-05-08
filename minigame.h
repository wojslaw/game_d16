#pragma once
#include "d16.h"

void
minigame_crafting(
		 int const level_stat
		,int const level_skill
		,int const progres_skill
		);


struct Character {
	int level_stat = 0;
	int level_skill = 0;
	int progres_skill = 0;
	int progres_next = 2;
	int resources = 10;
	void print();
	void add_progres(int const);
};


int
calc_progres_for_next_level(int const level);


struct Action {
	int modifier_difficulty;
	int cost;
	const char * name;
};




void
print_action(const Action &a);


void
print_actions_table(void) ;



extern const Action
TABLE_ACTION[];
extern size_t const SIZEOF_TABLE_ACTION;



