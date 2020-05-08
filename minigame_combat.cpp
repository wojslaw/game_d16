#include "minigame_combat.h"

enum weapon_type {
	weapon_type_none ,
	weapon_type_sword ,
	weapon_type_axe ,
	weapon_type_polearm ,
	weapon_type_club ,
	SIZEOF_STRINGTABLE_WEAPON_TYPE ,
};

const char *
STRINGTABLE_WEAPON_TYPE[] = {
	[weapon_type_none] = "[no_weapon_type]" ,
	[weapon_type_sword] = "sword",
	[weapon_type_axe] = "axe",
	[weapon_type_polearm] = "polearm",
	[weapon_type_club] = "club",
};
static_assert( SIZEOF_STRINGTABLE_WEAPON_TYPE == sizeof(STRINGTABLE_WEAPON_TYPE)/sizeof(STRINGTABLE_WEAPON_TYPE[0]) );


struct TrainingType {
	int modifier_difficulty;
	int cost;
	const char * name;
};


const char * STRING_HELP_TRAINING = "You advance with each failure of dice. Thus, if you want to gain higher skill levels, you have to pick more difficult training regimens.\n";
const
TrainingType
TABLE_TRAINING_TYPE[] {
	{  -1  ,   0  ,  "Hit a tree" } ,
	{  -3  ,   1  ,  "Hit a training dummy" } ,
	{  -6  ,   4  ,  "Hit a springy training dummy" } ,
	{  -8  ,   8  ,  "Train with a random person" } ,
	{ -15  ,  20  ,  "Train with a partner" } , // Idea: a partner who advances in skill!
	{ -20  ,  60  ,  "Train with an expert" } ,
};






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
	static
		const struct WeaponBase *
		weapon_base_get_pointer_from_id(
				size_t const id ) ;
};
const
WeaponBase
TABLE_WEAPON_BASE[] = {
	{ .type=weapon_type_none, .range=1, .base_damage=1, .name = "None" } ,
	{ .type=weapon_type_polearm , .to_hit = -3 , .range = 2, .base_damage = 2 , .name = "Training Spear" } ,
	{ .type=weapon_type_sword   , .to_hit = -3 , .range = 1, .base_damage = 2 , .name = "Training Sword" } ,
	{ .type=weapon_type_axe     , .to_hit = -3 , .range = 1, .base_damage = 2 , .name = "Training Axe" } ,
	{ .type=weapon_type_polearm , .range = 3, .base_damage = 6 , .name = "Spear" } ,
	{ .type=weapon_type_sword   , .range = 1, .base_damage = 3 , .name = "Shortsword" } ,
	{ .type=weapon_type_axe , .required_strength = 2 , .to_hit = -2 , .range = 1, .base_damage = 5 , .name = "Lumber Axe" } ,
};
size_t const SIZEOF_TABLE_WEAPON_BASE (sizeof(TABLE_WEAPON_BASE)/sizeof(TABLE_WEAPON_BASE[0])) ;


const struct WeaponBase *
WeaponBase::weapon_base_get_pointer_from_id(size_t const id)
{
	assert( id < SIZEOF_TABLE_WEAPON_BASE );
	return &(TABLE_WEAPON_BASE[id]);
}



struct Weapon {
	size_t weapon_base_id = 0;
	int quality = 0;
	int durability_max = 1;
	int durability_current = 1;
	void fprint(FILE * f);
};


void
Weapon::fprint(FILE * f) {
	fprintf( f , "weapon:{q%d,durability %d/%d,base[%#lx]:{"
			, quality
			, durability_max
			, durability_current 
			, weapon_base_id
		   );
	const struct WeaponBase * ptr_weaponbase = WeaponBase::weapon_base_get_pointer_from_id(weapon_base_id);
	ptr_weaponbase->fprint(f);
	fprintf( f , "}}" );
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
	fprintf( f , "{t %d rs %d rd %d rw %d th %d rng %d dmg %d"
			, type
			, required_strength
			, required_dexterity
			, required_wisdom
			, to_hit
			, range
			, base_damage );
	if( name ) {
		fprintf( f , "%s}", name );
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



struct Warrior {
	struct {
		int hp_max = 1;
		int hp_current = 1;
		int strength = 1;
		int dexterity = 1;
		int wisdom = 1;
	} stat;
	int hp_missing(void) const { return stat.hp_max - stat.hp_current;  }
	void equip();
};
