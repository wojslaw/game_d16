#include "minigame.h"



const Action
TABLE_ACTION[] = {
	{  0  , 1 ,  "Trivial"   ,  } ,
	{ -2  , 2 ,  "Easy"      ,  } ,
	{ -6  , 4 ,  "Medium"    ,  } ,
	{ -12 , 6 ,  "Difficult" ,  } ,
	{ -20 , 8 ,  "Advanced"  ,  } ,
};
size_t const SIZEOF_TABLE_ACTION = sizeof(TABLE_ACTION)/sizeof(TABLE_ACTION[0]);


const char *
STRINGTABLE_WEAPON_TYPE[] = {
	[weapon_type_none] = "[no_weapon_type]" ,
	[weapon_type_sword] = "sword",
	[weapon_type_axe] = "axe",
	[weapon_type_polearm] = "polearm",
	[weapon_type_club] = "club",
};
static_assert( SIZEOF_STRINGTABLE_WEAPON_TYPE == sizeof(STRINGTABLE_WEAPON_TYPE)/sizeof(STRINGTABLE_WEAPON_TYPE[0]) );



const
WeaponBase
TABLE_WEAPON_BASE[] = {
	{ .type=weapon_type_polearm , .range = 3, .base_damage = 6 , .name = "Spear" } ,
	{ .type=weapon_type_sword   , .range = 1, .base_damage = 3 , .name = "Shortsword" } ,
	{ .type=weapon_type_axe , .required_strength = 2 , .to_hit = -2 , .range = 1, .base_damage = 5 , .name = "Lumber Axe" } ,
};
size_t const SIZEOF_TABLE_WEAPON_BASE (sizeof(TABLE_WEAPON_BASE)/sizeof(TABLE_WEAPON_BASE[0])) ;






void Character::print() {
	printf( "(t %d; k %d; p %d/%d; $ %d)\n"
			,level_stat
			,level_skill
			,progres_skill
			,progres_next
			,resources);
}


int
calc_progres_for_next_level(int const level) {
	return (4 << (level/2));
}


void
Character::add_progres(int const delta_prog) {
	progres_skill += delta_prog;
	if( progres_skill >= progres_next ) {
		progres_skill = 0;
		++(level_skill);
		progres_next = calc_progres_for_next_level(level_skill);
	}
}




void
print_action(const Action &a){
	printf( "%3d   %2d  %s\n" , a.modifier_difficulty , a.cost , a.name  );
}

void
print_actions_table(void) {
	printf( " id  diff  cost  Description\n" );
	for(size_t i = 0;
			i < SIZEOF_TABLE_ACTION;
			++i) {
		printf( "%3lu  " , i );
		print_action(TABLE_ACTION[i]);
	}
}


void
display_table_weapon_type(FILE * f)
{
	for(int i = 0; i < SIZEOF_STRINGTABLE_WEAPON_TYPE; ++i) {
		fprintf(f , "%d %s\n" , i , STRINGTABLE_WEAPON_TYPE[i] );
	}
}


void
WeaponBase::fprint( FILE * f
) const {
	fprintf( f , "t %d rs %d rd %d rw %d th %d rng %d dmg %d"
			, type
			, required_strength
			, required_dexterity
			, required_wisdom
			, to_hit
			, range
			, base_damage );
	if( name ) {
		fprintf( f , "%s", name );
	}
}


void
display_table_weapon_base(FILE * f)
{
	for(size_t i = 0; i < SIZEOF_TABLE_WEAPON_BASE ; ++i ) {
		fprintf(f , "%lu" , i );
		TABLE_WEAPON_BASE[i].fprint(f);
		fprintf(f , "\n" );
	}
}

