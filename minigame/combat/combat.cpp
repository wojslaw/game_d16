#include "combat.h"



void
fprint_vector_of_strings(
		 FILE * f
		,std::vector< const char * > &vector_strings
		)
{
	fprintf( f ,  "\n" );
	int i = 0;
	for( auto s : vector_strings ) {
		fprintf( f , "%d %s\n"
				, i
				, s );
		++i;
	}
	if( (size_t)i != vector_strings.size() ) {
		fprintf( stderr , "BUG? in fprint_vector_of_strings(): (size_t)i != vector_strings.size()\n" );
	}
	if( i != (int)vector_strings.size() ) {
		fprintf( stderr , "BUG? in fprint_vector_of_strings(): i != (int)vector_strings.size()\n" );
	}
}


int
select_fprint_vector_of_strings(
		 FILE * f
		,std::vector< const char * > vector_strings
		)
{
	fprint_vector_of_strings(
			 f
			,vector_strings
			);
	fprintf( f , "input selection:\n" ); // TODO: better system for handling selections
	int selection = -1;
	scanf( "%i" , &selection );
	return selection;
}










enum weapon_type {
	weapon_type_none ,
	weapon_type_sword ,
	weapon_type_axe ,
	weapon_type_polearm ,
	weapon_type_club ,
	weapon_type_ranged_thrower ,
	SIZEOF_STRINGTABLE_WEAPON_TYPE ,
};


std::array<const char * , SIZEOF_STRINGTABLE_WEAPON_TYPE>
STRINGTABLE_WEAPON_TYPE = {{
	[weapon_type_none] = "[no_weapon_type]" ,
	[weapon_type_sword] = "sword",
	[weapon_type_axe] = "axe",
	[weapon_type_polearm] = "polearm",
	[weapon_type_club] = "club",
	[weapon_type_ranged_thrower] = "ranged_thrower" ,
}};


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

std::array<const MonsterType , SIZEOF_TABLE_MONSTER_TYPE>
TABLE_MONSTER_TYPE = {{
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
}};





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



const std::vector< WeaponBase > TABLE_WEAPON_BASE = {
	{ .type=weapon_type_none, .range=1, .base_damage=1, .name = "None" } ,
	{ .type=weapon_type_polearm , .to_hit = -3 , .range = 2, .base_damage = 2 , .name = "Training Spear" } ,
	{ .type=weapon_type_sword   , .to_hit = -3 , .range = 1, .base_damage = 2 , .name = "Training Sword" } ,
	{ .type=weapon_type_axe     , .to_hit = -3 , .range = 1, .base_damage = 2 , .name = "Training Axe" } ,
	{ .type=weapon_type_polearm , .range = 3, .base_damage = 6 , .name = "Spear" } ,
	{ .type=weapon_type_sword   , .range = 1, .base_damage = 3 , .name = "Shortsword" } ,
	{ .type=weapon_type_axe , .required_strength = 2 , .to_hit = -2 , .range = 1, .base_damage = 5 , .name = "Lumber Axe" } ,
	{ .type=weapon_type_ranged_thrower , .required_strength = 0 , .to_hit = 0 , .range = 6, .base_damage = 6 , .name = "Crossbow" } ,
	{ .type=weapon_type_ranged_thrower , .required_strength = 1 , .to_hit = -2 , .range = 5, .base_damage = 6 , .name = "Short Bow" } ,
	{ .type=weapon_type_ranged_thrower , .required_strength = 5 , .to_hit = -2 , .range = 8, .base_damage = 12 , .name = "Long Bow" } ,
};


const WeaponBase *
WeaponBase::weapon_base_get_pointer_from_id(size_t const id)
{
	assert( id < TABLE_WEAPON_BASE.size() );
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
	for(size_t i = 0; i < TABLE_WEAPON_BASE.size() ; ++i ) {
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
	counter_type_none ,
	counter_type_damage , // this will be "missing health", and replaces "hp_current"
	counter_type_poison ,
	counter_type_bleed ,
	counter_type_slowness ,
	counter_type_weakness ,
	COUNTER_TYPE_COUNT ,
};




std::array<const char * , COUNTER_TYPE_COUNT>
STRINGTABLE_COUNTERTYPE_SYMBOL = {{
	[counter_type_none] = "[[counter_type_none]]" ,
	[counter_type_damage] = "RD",
	[counter_type_poison] = "cP" ,
	[counter_type_bleed] = "cB" ,
	[counter_type_slowness] = "cS" ,
	[counter_type_weakness] = "cW" ,
}};


std::array<const char * , COUNTER_TYPE_COUNT>
STRINGTABLE_COUNTERTYPE_NAME {{
	[counter_type_none] = "[[counter_type_none]]" ,
	[counter_type_damage] = "ReceivedDamage",
	[counter_type_poison] = "Poison" ,
	[counter_type_bleed] = "Bleed" ,
	[counter_type_slowness] = "Slowness" ,
	[counter_type_weakness] = "Weakness"
}};


std::array<const char * , COUNTER_TYPE_COUNT>
STRINGTABLE_COUNTERTYPE_NAME_NEGATIVE = {{
	[counter_type_none] = "[[counter_type_none]]" ,
	[counter_type_damage] = "Overheal",
	[counter_type_poison] = "Antibodies(resist poison)" ,
	[counter_type_bleed] = "Clotting(resist bleed)" ,
	[counter_type_slowness] = "Quickness" ,
	[counter_type_weakness] = "Might"
}};


void
fprint_counter_full_description(
		 FILE * f
		,enum counter_type const ct
		,int const value)
{
	assert(ct > counter_type_none);
	assert(ct > COUNTER_TYPE_COUNT);
	const char * str
		= value >= 0
		? STRINGTABLE_COUNTERTYPE_NAME[ct]
		: STRINGTABLE_COUNTERTYPE_NAME_NEGATIVE[ct];
	int const normalized_value
		= value >= 0
		? value
		: -value ;
	fprintf( f , "(%s %d): %s %d"
			,STRINGTABLE_COUNTERTYPE_SYMBOL[ct]
			,normalized_value
			,str
			,value
			);
}


enum stat_type {
	stat_type_none ,
	stat_type_hp_max ,
	stat_type_hp_current ,
	stat_type_strength ,
	stat_type_dexterity ,
	stat_type_wisdom ,
	STAT_TYPE_COUNT ,
};


std::array<const char * , STAT_TYPE_COUNT>
STRINGTABLE_STATTYPE_SYMBOL = {{
	[stat_type_none] = "[[symbol:stat_type_none]]" ,
	[stat_type_hp_max] = "HPmax" ,
	[stat_type_hp_current] = "HPcur" ,
	[stat_type_strength] = "STR" ,
	[stat_type_dexterity] =  "DEX" ,
	[stat_type_wisdom] = "WIS" ,
}};


std::array<const char * , STAT_TYPE_COUNT>
STRINGTABLE_STATTYPE_NAME = {{
	[stat_type_none] = "[[name:stat_type_none]]" ,
	[stat_type_hp_max] = "Hitpoints Max" ,
	[stat_type_hp_current] = "Hitpoints Current" ,
	[stat_type_strength] = "Strength" ,
	[stat_type_dexterity] =  "Dexterity" ,
	[stat_type_wisdom] = "Wisdom" ,
}};


enum rollmod_type {
	rollmod_type_none ,
	rollmod_type_to_hit ,
	rollmod_type_damage ,
	rollmod_type_strength ,
	rollmod_type_dexterity ,
	rollmod_type_wisdom ,
	ROLLMOD_TYPE_COUNT ,
};


enum ability_type {
	ability_type_none ,
	ability_type_attack , // governed by the weapon, so the range(if I get to implement range) will be set to weapon's range
	ability_type_instant ,
	ability_type_apply_counters ,
	ABILITY_TYPE_COUNT ,
};


// idea: `vector_counter_type` and `vector_rollmod`
struct Ability {
	const char * name = "[[ability.name:none]]";
	enum ability_type type = ability_type_none;
	enum counter_type on_success_counter_type = counter_type_none;
	enum rollmod_type rollmod_type = rollmod_type_none;
	int rollmod_add = 0;
	int rollmod_multiply = 0;
	int range = 1;
	void fprint(FILE * f);
//	Ability() {};
//	Ability(
//			 const char * _name
//			,const enum ability_type _type
//			,enum rollmod_type _rollmod_type
//			,const int _rollmod_add
//			,const int _rollmod_multiply
//			,const enum counter_type _on_success_counter_type
//		   ) {
//		name
//			=
//			_name ;
//		type
//			=
//			_type ;
//		on_success_counter_type
//			=
//			_on_success_counter_type ;
//		rollmod_type
//			=
//			_rollmod_type;
//		rollmod_add
//			=
//			_rollmod_add ;
//		rollmod_multiply
//			=
//			_rollmod_multiply ;
//
//	}
};

void 
Ability::fprint(FILE * f) {
	fprintf( f , "%s(%d|%d) TH %d MULT %d"
			,name
			,type
			,on_success_counter_type
			,rollmod_add
			,rollmod_multiply
			);

}


typedef std::vector<const Ability *> VectorAbilityPointers;


enum ability_id {
	ability_none ,
	ability_attack ,
	ability_open_wounds ,
	ability_weaken ,
	ABILITY_COUNT ,
};



Ability ARRAY_ABILITIES[] = {
	{
		.name = "Attack" ,
		.type = ability_type_attack ,
		.on_success_counter_type = counter_type_none ,
		.rollmod_type = rollmod_type_to_hit ,
		.rollmod_add = 0 ,
		.rollmod_multiply = 0 ,
		.range = 1
	} ,
	{
		.name = "Open Wounds" ,
		.type = ability_type_attack ,
		.on_success_counter_type = counter_type_bleed ,
		.rollmod_type = rollmod_type_to_hit ,
		.rollmod_add = -4 ,
		.rollmod_multiply = 0 ,
		.range = 1
	} ,
	{
		.name = "Weaken" ,
		.type = ability_type_instant ,
		.on_success_counter_type = counter_type_weakness ,
		.rollmod_type = rollmod_type_wisdom ,
		.rollmod_add = -3 ,
		.rollmod_multiply = 0 ,
		.range = 1
	} ,
	{
		.name = "Induce Poison" ,
		.type = ability_type_instant ,
		.on_success_counter_type = counter_type_poison ,
		.rollmod_type = rollmod_type_wisdom ,
		.rollmod_add = -5 ,
		.rollmod_multiply = 1 ,
		.range = 1
	} ,
};




const VectorAbilityPointers
VECTOR_ABILITY_POINTERS_DEFAULT = {
	&(ARRAY_ABILITIES[0]) ,
	&(ARRAY_ABILITIES[1]) ,
	&(ARRAY_ABILITIES[2]) ,
};


std::vector< const char * >
vector_ability_pointer_get_vector_of_strings(
		const VectorAbilityPointers & vec)
{
	std::vector< const char * > vector_strings;
	for( auto a : vec ) {
		vector_strings.push_back( a->name );
	}
	return vector_strings;
}



struct CombatEntity {
	struct {
		int hp_max = 1;
		int hp_current = 1;
		int strength = 1;
		int dexterity = 1;
		int wisdom = 1;
	} stat;
	std::array< int , COUNTER_TYPE_COUNT > counter_array = {0};
	std::array< int , STAT_TYPE_COUNT > max_stat = {{
		[stat_type_none] = 1 ,
		[stat_type_hp_max] = 1 ,
		[stat_type_hp_current] = 1 ,
		[stat_type_strength] = 1 ,
		[stat_type_dexterity] = 1 ,
		[stat_type_wisdom] = 1 ,
	}};

	VectorAbilityPointers vector_available_abilities = VECTOR_ABILITY_POINTERS_DEFAULT;
	std::vector< const char * >
		vector_available_abilities_strings
		= vector_ability_pointer_get_vector_of_strings(
				vector_available_abilities );

	//methods
	void fprint(FILE * f);
	void fprint_hp(FILE * f);
	void fprint_all_counters(FILE * f);
	void fprint_nonzero_counters(FILE * f);

	bool is_dead() const { return ( stat.hp_current < 0); };

	int get_hp_missing(void) const {
		return (stat.hp_max
				 - stat.hp_current);  }
	int get_rollmod_strength(void) const {
		return (stat.strength
				 - get_counter_value(counter_type_weakness));
	};
	int get_rollmod_dexterity(void) const {
		return (stat.dexterity
			 - get_counter_value(counter_type_weakness));
	};
	int get_rollmod_wisdom(void) const { return stat.wisdom; };
	int get_rollmod_to_hit(void) const  { return get_rollmod_dexterity(); };
	int get_rollmod_defense(void) const { return get_rollmod_dexterity(); };
	int get_rollmod_damage(void) const { return get_rollmod_strength(); };
	int get_counter_value(enum counter_type ct) const;
	int * ref_counter(enum counter_type ct);
	int get_rollmod(enum rollmod_type const rt);
	int get_stat(enum stat_type const st);
	int get_max_stat(enum stat_type const st);

	void receive_damage(int damage);
	void receive_attack(const CombatEntity * attacker);

	void perform_post_counters_per_round_effects(void);

	void equip(); //todo
};


int CombatEntity::get_rollmod(enum rollmod_type const rt) {
	int rollmod = 0;
	switch(rt) {
		case rollmod_type_none:
			break;
		case rollmod_type_to_hit:
			rollmod = get_rollmod_to_hit();
			break;
		case rollmod_type_damage:
			rollmod = get_rollmod_damage();
			break;
		case rollmod_type_strength:
			rollmod = get_rollmod_strength();
			break;
		case rollmod_type_dexterity:
			rollmod = get_rollmod_dexterity();
			break;
		case rollmod_type_wisdom:
			rollmod = get_rollmod_wisdom();
			break;
		default:
			fprintf( stderr , "get_rollmod:unexpected rollmod_type = %d" , rt );
	}
	return rollmod;
}


int
CombatEntity::get_stat(enum stat_type const st) {
	assert( st > stat_type_none );
	assert( st < STAT_TYPE_COUNT );
	return 0; // TODO
}


int
CombatEntity::get_max_stat(enum stat_type const st) {
	assert( st > stat_type_none );
	assert( st < STAT_TYPE_COUNT );
	return max_stat[st];
}


RollResult roll_attack(
		 const CombatEntity & attacker
		,const CombatEntity & target
		) {
	int const rollmod_attack = attacker.get_rollmod_to_hit();
	int const rollmod_defense = target.get_rollmod_defense();
	int const rollmod_damage = attacker.get_rollmod_damage();
	return RollResult(
			(rollmod_attack - rollmod_defense)
			,rollmod_damage ) ;
}


/*struct EffectToApply {
	RollResult 
};

EffectToApply
calculate_ability_use(
		enum ability_id
		,const CombatEntity & attacker
		,const CombatEntity & target)
{
	switch(ability_id) {
		
		default:
			fprintf( stderr , "[[unexpected ability_id: %d]]" , ability_type);
	}
}*/



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


void
CombatEntity::fprint_hp(FILE * f) {
	fprintf( f , "(hp: %d/%d)"
			,stat.hp_current
			,stat.hp_max
			);
}


void CombatEntity::receive_damage(int damage) {
	if( damage < 0 ) {
		damage = 0;
	}
	stat.hp_current -= damage;
	(*ref_counter(counter_type_damage)) += damage;
}



typedef int counter_function_reduction(int * counter);

counter_function_reduction counter_perform_halving;
counter_function_reduction counter_perform_decrement;

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


int counter_perform_decrement(int * counter) {
	if((*counter) > 0) {
		--(*counter);
	}
	return (*counter);
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
	counter_perform_decrement(ref_counter(counter_type_slowness));
	counter_perform_decrement(ref_counter(counter_type_weakness));
}


void
perform_example_combat(FILE * f)
{
	struct CombatEntity you;
	you.stat.dexterity = 3;
	you.stat.strength = 4;
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
		int selection = select_fprint_vector_of_strings( stdout , you.vector_available_abilities_strings ); //TODO
		if( selection < 0 || selection >= (int)you.vector_available_abilities.size() ) { // I'm (probably unneccesarily) afraid of casting here
			fprintf( f , "aborting, since you selected %d\n" , selection);
			goto jump_end;
		}
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
jump_end:
	fprintf( f, "  Combat ended after %d rounds. Results:\n" , round );
	fprintf( f , "you: " );
	you.fprint(f);
	fprintf( f , "\n" );
	fprintf( f , "foe: " );
	foe.fprint(f);
	fprintf( f , "\n" );
}


