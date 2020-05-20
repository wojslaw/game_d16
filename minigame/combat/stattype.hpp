#pragma once
#include <stdio.h>
#include <array>


enum stat_type {
	stat_type_none ,
	stat_type_strength ,
	stat_type_dexterity ,
	stat_type_wisdom ,
	STAT_TYPE_COUNT ,
};

typedef int statarray[STAT_TYPE_COUNT];



extern std::array<const char * , STAT_TYPE_COUNT>
STRINGTABLE_STATTYPE_SYMBOL;

extern std::array<const char * , STAT_TYPE_COUNT>
STRINGTABLE_STATTYPE_NAME;


void
fprint_stat_array(
		FILE * f
		, const statarray &sa );
