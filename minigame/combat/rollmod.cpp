#include "rollmod.hpp"



const char * 
STRINGTABLE_ROLLMOD_NAME[ROLLMOD_TYPE_COUNT] = {
	[rollmod_type_none]        = "[[rollmod_type_none]]" ,
	[rollmod_type_vitality]    = "vitality"  ,
	[rollmod_type_attack]      = "attack"    ,
	[rollmod_type_magic]       = "magic"     ,
	[rollmod_type_to_hit]      = "to_hit"    ,
	[rollmod_type_defense]     = "defense"   ,
	[rollmod_type_damage]      = "damage"    ,
	[rollmod_type_strength]    = "strength"  ,
	[rollmod_type_dexterity]   = "dexterity" ,
	[rollmod_type_wisdom]      = "wisdom"    ,
};


const char * 
STRINGTABLE_ROLLMOD_SHORTNAME[ROLLMOD_TYPE_COUNT] = {
	[rollmod_type_none]        = "[[shortname:rollmod_type_none]]" ,
	[rollmod_type_vitality]    = "VIT" ,
	[rollmod_type_attack]      = "ATT" ,
	[rollmod_type_magic]       = "MAG" ,
	[rollmod_type_to_hit]      = "HIT" ,
	[rollmod_type_defense]     = "DEF" ,
	[rollmod_type_damage]      = "DMG" ,
	[rollmod_type_strength]    = "STR" ,
	[rollmod_type_dexterity]   = "DEX" ,
	[rollmod_type_wisdom]      = "WIS" ,
};

void
fprint_rollmod_array(
		 FILE * f
		,const int (&arr)[ROLLMOD_TYPE_COUNT]) {
	size_t i = 0;
	for( const auto &v : arr ) {
		if( v != 0 ) {
			
		}
		fprintf( f , "%s %d"
				, STRINGTABLE_ROLLMOD_SHORTNAME[i]
				, v);
		++i;
	}
}



