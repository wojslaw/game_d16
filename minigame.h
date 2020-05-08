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


enum weapon_type {
	weapon_type_none ,
	weapon_type_sword ,
	weapon_type_axe ,
	weapon_type_polearm ,
	weapon_type_club ,
	SIZEOF_STRINGTABLE_WEAPON_TYPE ,
};

extern const char * STRINGTABLE_WEAPON_TYPE[];



void
display_table_weapon_type(FILE * f);


struct WeaponBase {
	enum weapon_type type = weapon_type_none;
	int required_strength = 0;
	int required_dexterity = 0;
	int required_wisdom = 0;
	int to_hit = 0;
	int range = 0;
	int base_damage = 0;
	const char * name = NULL;
	void fprint(FILE * f) const;
};


extern const
WeaponBase
TABLE_WEAPON_BASE[];
extern size_t const SIZEOF_TABLE_WEAPON_BASE;



extern const Action
TABLE_ACTION[];
extern size_t const SIZEOF_TABLE_ACTION;



void
display_table_weapon_base(FILE * f);
