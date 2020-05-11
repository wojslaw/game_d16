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


enum counter_type {
 // I keep wanting to add some harder abstraction for the counters.
 // as in: a separate definition of counter_type
 // that would hold data on "is it halving per turn, or decrement. does it deal damage? and so on", to decouple things further
 // I also want to put all the stats in an array, to also have stat_type_strength etc.
	counter_type_poison ,
	counter_type_bleed ,
	counter_type_slowness ,
	counter_type_weakness ,
	COUNTER_TYPE_COUNT ,
};

const char *
STRINGTABLE_COUNTERTYPE_SYMBOL[COUNTER_TYPE_COUNT] {
	[counter_type_poison] = "Pois" ,
	[counter_type_bleed] = "Bled" ,
	[counter_type_slowness] = "Slow" ,
	[counter_type_weakness] = "Weak"
};


const char *
STRINGTABLE_COUNTERTYPE_NAME[COUNTER_TYPE_COUNT] {
	[counter_type_poison] = "Poison" ,
	[counter_type_bleed] = "Bleed" ,
	[counter_type_slowness] = "Slowness" ,
	[counter_type_weakness] = "Weakness"
};


enum stat_type {
	stat_type_hp_max ,
	stat_type_hp_current ,
	stat_type_strength ,
	stat_type_dexterity ,
	stat_type_wisdom ,
	STAT_TYPE_COUNT ,
};


const char *
STRINGTABLE_STATTYPE_SYMBOL[STAT_TYPE_COUNT] {
	[stat_type_hp_max] = "HPmax" ,
	[stat_type_hp_current] = "HPcur" ,
	[stat_type_strength] = "STR" ,
	[stat_type_dexterity] =  "DEX" ,
	[stat_type_wisdom] = "WIS" ,
};


const char *
STRINGTABLE_STATTYPE_NAME[STAT_TYPE_COUNT] {
	[stat_type_hp_max] = "Hitpoints Max" ,
	[stat_type_hp_current] = "Hitpoints Current" ,
	[stat_type_strength] = "Strength" ,
	[stat_type_dexterity] =  "Dexterity" ,
	[stat_type_wisdom] = "Wisdom" ,
};


struct CombatEntity {
	struct {
		int hp_max = 1;
		int hp_current = 1;
		int strength = 1;
		int dexterity = 1;
		int wisdom = 1;
	} stat;
	int counter_array[COUNTER_TYPE_COUNT] = {0};

	//methods
	void fprint(FILE * f);
	void fprint_all_counters(FILE * f);
	void fprint_nonzero_counters(FILE * f);

	bool is_dead() const { return ( stat.hp_current < 0); };

	int get_hp_missing(void) const { return stat.hp_max - stat.hp_current;  }
	int get_rollmod_strength(void) const { return (stat.strength); };
	int get_rollmod_dexterity(void) const { return (stat.dexterity - get_counter_value(counter_type_weakness)); };
	int get_rollmod_wisdom(void) const { return stat.wisdom; };
	int get_rollmod_attack(void) const  { return get_rollmod_dexterity(); };
	int get_rollmod_defense(void) const { return get_rollmod_dexterity(); };
	int get_rollmod_damage(void) const { return get_rollmod_strength(); };
	int get_counter_value(enum counter_type ct) const;
	int * ref_counter(enum counter_type ct);

	void receive_damage(int damage);
	void receive_attack(const CombatEntity * attacker);

	void perform_post_counters_per_round_effects(void);

	void equip(); //todo
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
	fprintf( f , "hp %d/%d , STR %d , DEX %d , WIS %d ;"
			,stat.hp_current
			,stat.hp_max
			,stat.strength
			,stat.dexterity
			,stat.wisdom
			);
	fprint_nonzero_counters(f);
}


void
CombatEntity::fprint_nonzero_counters(FILE * f)  {
	bool printed_some_counter = false;
	for( int i = 0; i < COUNTER_TYPE_COUNT; ++i ) {
		int const value = get_counter_value( (enum counter_type) i );
		if( value != 0 ) {
			if( !printed_some_counter ) {
				fprintf( f , "(counters:");
				printed_some_counter = true;
			}
			fprintf( f , " %s%d"
					, STRINGTABLE_COUNTERTYPE_SYMBOL[i]
					, value  );
		}
	}
	if( printed_some_counter ) {
		fprintf( f , ") ;" );
	}
}


void
CombatEntity::fprint_all_counters(FILE * f)  {
	for( int i = 0; i < COUNTER_TYPE_COUNT; ++i ) {
		fprintf( f , " c%s%d"
				, STRINGTABLE_COUNTERTYPE_SYMBOL[i]
				, get_counter_value( (enum counter_type) i ) );
	}
}


void CombatEntity::receive_damage(int damage) {
	if( damage < 0 ) {
		damage = 0;
	}
	stat.hp_current -= damage;
}


int
counter_perform_halving(int * counter) {
	if( (*counter) > 0 ) {
		int value = (*counter)/2;
		if( value < 1 ) {
			value = 1;
		}
		(*counter) -= value;
		return value;
	}

	if( *counter < 0 ) {
		++(*counter);
	}
	return 0; // I'm not sure about handling negative values. For now, I think it will be enough if they just get returned as 0, since "halving"-type counters are damage.
}


void counter_decrement(int * counter) {
	if((*counter) > 0) {
		--(*counter);
	}
}


int * CombatEntity::ref_counter(enum counter_type ct) {
	assert( ct >= 0);
	assert( ct < COUNTER_TYPE_COUNT);
	return &(counter_array[ct]);
}


int CombatEntity::get_counter_value(enum counter_type ct) const {
	assert( ct >= 0);
	assert( ct < COUNTER_TYPE_COUNT);
	return counter_array[ct];
}


void
CombatEntity::perform_post_counters_per_round_effects(void) {
	// damage-over-time
	receive_damage( counter_perform_halving(ref_counter(counter_type_poison)) );
	receive_damage( counter_perform_halving(ref_counter(counter_type_bleed)) );
	// only reduce
	counter_decrement(ref_counter(counter_type_slowness));
	counter_decrement(ref_counter(counter_type_weakness));
}


void
perform_example_combat(FILE * f)
{
	struct CombatEntity you;
	you.stat.dexterity = 3;
	you.stat.strength = 0;
	you.stat.hp_max = 16;
	you.stat.hp_current = 16;
	struct CombatEntity foe;
	foe.stat.hp_max = 12;
	foe.stat.hp_current = 12;
	foe.stat.strength = 4;
	(*foe.ref_counter(counter_type_bleed)) = 2;
	(*foe.ref_counter(counter_type_weakness)) = 4;

	fprintf( f , "you: " );
	you.fprint(f);
	fprintf( f , "\n" );
	fprintf( f , "foe: " );
	foe.fprint(f);
	fprintf( f , "\n" );
	int const ROUND_COUNT = 4;

	int round = 0;
	for( ; round < ROUND_COUNT ; ++round ) {
		fprintf(f , "  round %d\n" , round);
		fprintf( f, "you hit:" );
		const RollResult rr_atk_you = roll_attack( you , foe );
		rr_atk_you.fprint(f);
		fprintf( f , "\n" );
		fprintf( f, "foe hits:" );
		const RollResult rr_atk_foe = roll_attack( foe , you );
		rr_atk_foe.fprint(f);
		fprintf( f , "\n" );
		foe.receive_damage( rr_atk_you.get_success_level() );
		you.receive_damage( rr_atk_foe.get_success_level() );
		if( you.is_dead() || foe.is_dead() ) {
			break;
		}
		you.perform_post_counters_per_round_effects();
		foe.perform_post_counters_per_round_effects();
		fprintf( f , "you: " );
		you.fprint(f);
		fprintf( f , "\n" );
		fprintf( f , "foe: " );
		foe.fprint(f);
		fprintf( f , "\n" );
	}
	fprintf( f, "  Combat ended after %d rounds. Results:\n" , round );
	fprintf( f , "you: " );
	you.fprint(f);
	fprintf( f , "\n" );
	fprintf( f , "foe: " );
	foe.fprint(f);
	fprintf( f , "\n" );
}


