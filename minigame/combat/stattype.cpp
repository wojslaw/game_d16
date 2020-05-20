#include "stattype.hpp"

std::array<const char * , STAT_TYPE_COUNT>
STRINGTABLE_STATTYPE_SYMBOL = {{
	[stat_type_none] = "[[symbol:stat_type_none]]" ,
	[stat_type_strength] = "STR" ,
	[stat_type_dexterity] =  "DEX" ,
	[stat_type_wisdom] = "WIS" ,
}};


std::array<const char * , STAT_TYPE_COUNT>
STRINGTABLE_STATTYPE_NAME = {{
	[stat_type_none] = "[[name:stat_type_none]]" ,
	[stat_type_strength] = "Strength" ,
	[stat_type_dexterity] =  "Dexterity" ,
	[stat_type_wisdom] = "Wisdom" ,
}};




void
fprint_stat_array(
		FILE * f
		, const statarray &sa ) {
	size_t i = 0;
	for( const auto &a : sa ) {
		if( a != 0 ) {
			fprintf( f , "%s %d "
					,STRINGTABLE_STATTYPE_SYMBOL[i]
					,a
				   );
			++i;
		}
	}
}
