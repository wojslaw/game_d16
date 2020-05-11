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
STRINGTABLE_WEAPON_TYPE[SIZEOF_STRINGTABLE_WEAPON_TYPE] = {
	[weapon_type_none] = "[no_weapon_type]" ,
	[weapon_type_sword] = "sword",
	[weapon_type_axe] = "axe",
	[weapon_type_polearm] = "polearm",
	[weapon_type_club] = "club",
};
static_assert( SIZEOF_STRINGTABLE_WEAPON_TYPE == sizeof(STRINGTABLE_WEAPON_TYPE)/sizeof(STRINGTABLE_WEAPON_TYPE[0]) );


enum monster_type {
	monster_type_none ,
	monster_type_devil ,
	monster_type_blob ,
	monster_type_porkish ,
	monster_type_mootant ,
	monster_type_undead ,
	monster_type_vermin ,
	SIZEOF_TABLE_MONSTER_TYPE ,
};

struct MonsterType {
	int base_hp_max = 0;
	int base_damage = 1;
	int base_attack = 1;
	int base_defense = 1;
	const char * description;
};


// maybe it isn't a good idea to put in those "base stats" for monster types?
// I'm thinking about whether I want to balance it around different monster_type having differing levels of difficulty?
// Or maybe! The "base stats" will be more of a "focus" of each monster_type
// one monster_type would be low-health, high def? other would be low hp/def, but high attack?
// and other would be more "human like" in that they are well-rounded
// and only specific monster_base in a given monster_type would be even further specialized.
// but I still have a feeling this is a stupid idea, and that it will be overall more sensible to define stats for each monster_type, without shadowing that by 
// because I don't exactly want to feel stuck in having to think about both the monster_type and monster_base stats
// but still, it sounds like a mildly interesting idea, to think of it kinda like species
// There could also be something kinda like Diablo, in that there are so-called difficulty levels, which just mean the monsters get higher stats there.
const MonsterType
TABLE_MONSTER_TYPE[SIZEOF_TABLE_MONSTER_TYPE] = {
	[monster_type_none] = {
		.description = "None"
	},
	[monster_type_devil] = {
		.description = "Devil"
	},
	[monster_type_blob] = {
		.description = "Blob"
	},
	[monster_type_porkish] = {
		.base_hp_max  = 2 ,
		.base_damage  = 2 ,
		.base_attack  = 0 ,
		.base_defense = 4 ,
		.description = "Porkish"
	},
	[monster_type_mootant] = {
		.description = "Mootant"
	},
	[monster_type_undead] = {
		.base_hp_max  = 0 ,
		.base_damage  = 2 ,
		.base_attack  = 2 ,
		.base_defense = 4 ,
		.description  = "Undead"
	},
	[monster_type_vermin] = {
		.base_hp_max  = 0 ,
		.base_damage  = 2 ,
		.base_attack  = 0 ,
		.base_defense = 4 ,
		.description  = "Vermin" ,
	},
};
static_assert( SIZEOF_TABLE_MONSTER_TYPE == sizeof(TABLE_MONSTER_TYPE)/sizeof(TABLE_MONSTER_TYPE[0]) );





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



struct CombatEntity {
	struct {
		int hp_max = 1;
		int hp_current = 1;
		int strength = 1;
		int dexterity = 1;
		int wisdom = 1;
	} stat;
	int hp_missing(void) const { return stat.hp_max - stat.hp_current;  }
	int get_rollmod_strength(void) const { return stat.strength; };
	int get_rollmod_dexterity(void) const { return stat.dexterity; };
	int get_rollmod_wisdom(void) const { return stat.wisdom; };
	int get_rollmod_attack(void) const  { return get_rollmod_dexterity(); };
	int get_rollmod_defense(void) const { return get_rollmod_dexterity(); };
	int get_rollmod_damage(void) const { return get_rollmod_strength(); };
	void equip(); //todo
	void fprint(FILE * f);
	void receive_damage(int damage);
	void receive_attack(const CombatEntity * attacker);
};


RollResult roll_attack(
		 const CombatEntity & attacker
		,const CombatEntity & target
		) {
	int const rollmod_attack = attacker.get_rollmod_attack();
	int const rollmod_defense = target.get_rollmod_defense();
	int const rollmod_damage = attacker.get_rollmod_damage();
	return RollResult(
			(rollmod_attack - rollmod_defense)
			,rollmod_damage ) ;
}


void
CombatEntity::fprint(FILE * f) {
	fprintf( f , "hp %d/%d , STR %d , DEX %d , WIS %d"
			,stat.hp_current
			,stat.hp_max
			,stat.strength
			,stat.dexterity
			,stat.wisdom
			);
}


void CombatEntity::receive_damage(int damage) {
	if( damage < 0 ) {
		damage = 0;
	}
	stat.hp_current -= damage;
}



void
perform_example_combat(FILE * f)
{
	struct CombatEntity you;
	you.stat.dexterity = 3;
	you.stat.strength = 6;
	you.stat.hp_max = 8;
	you.stat.hp_current = 8;
	struct CombatEntity foe;
	foe.stat.hp_max = 4;
	foe.stat.hp_current = 4;
	foe.stat.strength = 4;

	fprintf( f , "you: " );
	you.fprint(f);
	fprintf( f , "\n" );
	fprintf( f , "foe: " );
	foe.fprint(f);
	fprintf( f , "\n" );
	int const ROUND_COUNT = 4;

	for( int i = 0; i < ROUND_COUNT ; ++i ) {
		fprintf(f , "round %d\n" , i);
			fprintf( f, "you hit:" );
			RollResult atk_you = roll_attack( you , foe );
			atk_you.fprint(f);
			fprintf( f , "\n" );
			fprintf( f, "foe hits:" );
			RollResult atk_foe = roll_attack( foe , you );
			atk_foe.fprint(f);
			fprintf( f , "\n" );
			
			fprintf( f , "\n" );
	}
}


