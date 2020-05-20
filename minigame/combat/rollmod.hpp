#pragma once
#include <stdio.h>


int const STRENGTH_MULTIPLIER_VITALITY = 2;
int const VITALITY_BASE = 4;
int const VITALITY_MINIMUM = 1;


enum rollmod_type {
	rollmod_type_none ,
	rollmod_type_vitality ,
	rollmod_type_attack ,
	rollmod_type_magic ,
	rollmod_type_to_hit ,
	rollmod_type_defense ,
	rollmod_type_damage ,
	rollmod_type_strength ,
	rollmod_type_dexterity ,
	rollmod_type_wisdom ,
	ROLLMOD_TYPE_COUNT ,
};


extern const char * 
STRINGTABLE_ROLLMOD_NAME[ROLLMOD_TYPE_COUNT];


extern const char * 
STRINGTABLE_ROLLMOD_SHORTNAME[ROLLMOD_TYPE_COUNT];

void
fprint_rollmod_array(
		 FILE * f
		,const int (&arr)[ROLLMOD_TYPE_COUNT]);
