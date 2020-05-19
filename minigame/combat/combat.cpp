#include "combat.h"



enum item_type {
	item_type_none ,
	item_type_wearable ,
	item_type_consumable ,
	item_type_permanent ,
	item_type_sword ,
	item_type_axe ,
	item_type_polearm ,
	item_type_club ,
	item_type_throwable ,
	item_type_ranged_thrower ,
	COUNT_ITEM_TYPE ,
};


enum weapon_type {
	weapon_type_none ,
	weapon_type_sword ,
	weapon_type_axe ,
	weapon_type_polearm ,
	weapon_type_club ,
	weapon_type_throwable ,
	weapon_type_ranged_thrower ,
	COUNT_WEAPON_TYPE
};


const char *
STRINGTABLE_ITEM_TYPE[COUNT_ITEM_TYPE] = {
	[item_type_none]
		= "item_type_none" ,
	[item_type_wearable]
		= "item_type_wearable" ,
	[item_type_consumable]
		= "item_type_consumable" ,
	[item_type_permanent]
		= "item_type_permanent" ,
	[item_type_sword]
		= "item_type_sword" ,
	[item_type_axe]
		= "item_type_axe" ,
	[item_type_polearm]
		= "item_type_polearm" ,
	[item_type_club]
		= "item_type_club" ,
	[item_type_throwable]
		= "item_type_throwable" ,
	[item_type_ranged_thrower]
		= "item_type_ranged_thrower" ,
};


enum slot_type {
	slot_type_none ,
	slot_type_hand ,
	slot_type_offhand ,
	slot_type_head ,
	slot_type_body ,
	slot_type_legs ,
	slot_type_ring ,
	COUNT_SLOT_TYPE ,
};


std::array<const char * , COUNT_SLOT_TYPE>
STRINGTABLE_SLOT_TYPE = {{
	[slot_type_none] = "none" ,
	[slot_type_hand] = "hand" ,
	[slot_type_offhand] = "offhand" ,
	[slot_type_head] = "head" ,
	[slot_type_body] = "body" ,
	[slot_type_legs] = "legs" ,
	[slot_type_ring] = "ring" ,
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


enum targeting_type {
	targeting_type_none ,
	targeting_type_self ,
	targeting_type_enemy ,
	targeting_type_friend ,
	targeting_type_area ,
	TARGETING_TYPE_COUNT ,
};

const char *
STRINGTABLE_TARGETING_TYPE[TARGETING_TYPE_COUNT] = {
	[targeting_type_none]    = "[[targeting_type_none]]" ,
	[targeting_type_self]    = "self" ,
	[targeting_type_enemy]   = "enemy" ,
	[targeting_type_friend]  = "friend" ,
	[targeting_type_area]    = "area" ,
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
	stat_type_strength ,
	stat_type_dexterity ,
	stat_type_wisdom ,
	STAT_TYPE_COUNT ,
};


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
int const STRENGTH_MULTIPLIER_VITALITY = 2;
int const VITALITY_BASE = 4;
int const VITALITY_MINIMUM = 1;

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



enum skill {
	skill_none ,
	skill_alchemistry ,
	skill_mathemagics ,
	SKILL_COUNT ,
};

const char *
STRINGTABLE_SKILL[SKILL_COUNT] = {
	[skill_none] = "[[skill_none]]" ,
	[skill_alchemistry] = "Alchemistry",
	[skill_mathemagics] = "Mathemagics" ,
};





struct ItemBase {
	enum item_type type = item_type_none;
	bool is_2handed = false;
	bool is_breakable = true;
	int default_max_durability = 1;
	int required_stat[STAT_TYPE_COUNT] = { 0 };
	int range = 0;
	int rollmod[ROLLMOD_TYPE_COUNT] = { 0 };
	const char * name = NULL;
	void fprint(FILE * f) const;
	static
		const struct ItemBase *
		item_base_get_pointer_from_id(
				size_t const id ) ;
	int get_rollmod(enum rollmod_type rt) const;
};


int
ItemBase::get_rollmod(enum rollmod_type rt) const {
	assert( rt > rollmod_type_none );
	assert( rt < ROLLMOD_TYPE_COUNT );
	return rollmod[rt];
}


int const WEAPON_TRAINING_TO_HIT = -3;
int const WEAPON_TRAINING_DAMAGE =  3;


int
ARRAY_ROLLMOD_WEAPON_TRAINING[ROLLMOD_TYPE_COUNT]  = {
	[rollmod_type_none     ]  = 0 ,
	[rollmod_type_vitality ]  = 0 ,
	[rollmod_type_attack   ]  = 0 ,
	[rollmod_type_magic    ]  = 0 ,
	[rollmod_type_to_hit   ]  = WEAPON_TRAINING_TO_HIT ,
	[rollmod_type_defense  ]  = 0 ,
	[rollmod_type_damage   ]  = WEAPON_TRAINING_DAMAGE ,
	[rollmod_type_strength ]  = 0 ,
	[rollmod_type_dexterity]  = 0 ,
	[rollmod_type_wisdom   ]  = 0 ,
} ;



const  ItemBase
TABLE_ITEM_BASE[] = {

	{
		type : item_type_none,
		range : 1,
		name  :  "None" 
	},

	{
		type : item_type_polearm ,
		is_2handed : true ,
		default_max_durability : 6 ,
		range  :  2,
		rollmod  : {
			[rollmod_type_none     ]  = 0 ,
			[rollmod_type_vitality ]  = 0 ,
			[rollmod_type_attack   ]  = 0 ,
			[rollmod_type_magic    ]  = 0 ,
			[rollmod_type_to_hit   ]  = WEAPON_TRAINING_TO_HIT ,
			[rollmod_type_defense  ]  = 0 ,
			[rollmod_type_damage   ]  = WEAPON_TRAINING_DAMAGE ,
			[rollmod_type_strength ]  = 0 ,
			[rollmod_type_dexterity]  = 0 ,
			[rollmod_type_wisdom   ]  = 0 ,
		} ,
		name  :  "Training Spear" ,
	},

	{
		type : item_type_sword   ,
		default_max_durability : 6 ,
		range  :  1,
		rollmod  : {
			[rollmod_type_none     ]  = 0 ,
			[rollmod_type_vitality ]  = 0 ,
			[rollmod_type_attack   ]  = 0 ,
			[rollmod_type_magic    ]  = 0 ,
			[rollmod_type_to_hit   ]  = WEAPON_TRAINING_TO_HIT ,
			[rollmod_type_defense  ]  = 0 ,
			[rollmod_type_damage   ]  = WEAPON_TRAINING_DAMAGE ,
			[rollmod_type_strength ]  = 0 ,
			[rollmod_type_dexterity]  = 0 ,
			[rollmod_type_wisdom   ]  = 0 ,
		} ,
		name  :  "Training Sword" ,
	},
	{
		type : item_type_axe     ,
		is_2handed : true ,
		default_max_durability : 8 ,
		range  :  1,
		rollmod  : {
			[rollmod_type_none     ]  = 0 ,
			[rollmod_type_vitality ]  = 0 ,
			[rollmod_type_attack   ]  = 0 ,
			[rollmod_type_magic    ]  = 0 ,
			[rollmod_type_to_hit   ]  = WEAPON_TRAINING_TO_HIT ,
			[rollmod_type_defense  ]  = 0 ,
			[rollmod_type_damage   ]  = WEAPON_TRAINING_DAMAGE ,
			[rollmod_type_strength ]  = 0 ,
			[rollmod_type_dexterity]  = 0 ,
			[rollmod_type_wisdom   ]  = 0 ,
		} ,
		name  :  "Training Axe" 
	},

	{
		type : item_type_polearm ,
		is_2handed : true ,
		default_max_durability : 8 ,
		range  :  2 ,
		rollmod  : {
			[rollmod_type_none     ]  = 0 ,
			[rollmod_type_vitality ]  = 0 ,
			[rollmod_type_attack   ]  = 0 ,
			[rollmod_type_magic    ]  = 0 ,
			[rollmod_type_to_hit   ]  = 0 ,
			[rollmod_type_defense  ]  = 0 ,
			[rollmod_type_damage   ]  =  6 ,
			[rollmod_type_strength ]  = 0 ,
			[rollmod_type_dexterity]  = 0 ,
			[rollmod_type_wisdom   ]  = 0 ,
		} ,
		name  :  "Spear" 
	},

	{
		type : item_type_sword   ,
		default_max_durability : 12 ,
		range  :  1 ,
		rollmod  : {
			[rollmod_type_none     ]  = 0 ,
			[rollmod_type_vitality ]  = 0 ,
			[rollmod_type_attack   ]  = 0 ,
			[rollmod_type_magic    ]  = 0 ,
			[rollmod_type_to_hit   ]  = 0 ,
			[rollmod_type_defense  ]  = 0 ,
			[rollmod_type_damage   ]  =  4 ,
			[rollmod_type_strength ]  = 0 ,
			[rollmod_type_dexterity]  = 0 ,
			[rollmod_type_wisdom   ]  = 0 ,
		} ,
		name  :  "Shortsword" 
	},

/* 	{ */
/* 		type : item_type_axe , */
/* 		//required_strength  :  2 , */
/* 		to_hit  : -4 , */
/* 		/1* rollmod  :  { *1/ */
/* 		/1* 	[rollmod_type_to_hit] = -4 , *1/ */
/* 		/1* 	[rollmod_type_damage] =  5 , *1/ */
/* 		/1* }, *1/ */
/* 		name  :  "Lumber Axe" */ 
/* 	}, */

/* 	{ */
/* 		type : item_type_ranged_thrower , */
/*		is_2handed : true , */
/* 		to_hit  :  0 , */
/* 		/1* rollmod  :  { *1/ */
/* 		/1* 	[rollmod_type_damage] =  6 , *1/ */
/* 		/1* }, *1/ */
/* 		name  :  "Crossbow" */ 
/* 	}, */

/* 	{ */
/* 		type : item_type_ranged_thrower , */
/*		is_2handed : true , */
/* 		range  :  5, */
/* 		/1* rollmod  :  { *1/ */
/* 		/1* 	[rollmod_type_damage] = -2 , *1/ */
/* 		/1* 	[rollmod_type_damage] =  6 , *1/ */
/* 		/1* }, *1/ */
/* 		name  :  "Short Bow" */ 
/* 	}, */
/* 	{ */
/* 		type : item_type_ranged_thrower , */
/*		is_2handed : true , */
/* 		range  :  8, */
/* 		//rollmod  :  { */
/* 		//	[rollmod_type_damage] = -2 , */
/* 		//	[rollmod_type_damage] =  8 , */
/* 		//}, */
/* 		name  :  "Long Bow" */ 
/* 	}, */

/* 	{ */
/* 		type : item_type_throwable , */
/*		is_2handed : true , */
/* 		range  :  4, */
/* 		//rollmod  :  { */
/* 		//	[rollmod_type_to_hit] = -2 , */
/* 		//	[rollmod_type_damage] =  8 , */
/* 		//}, */
/* 		name  :  "ThrowableStone" */
/* 	} , */
/* }; */
	};


int const ITEM_BASE_COUNT = sizeof(TABLE_ITEM_BASE) / sizeof(TABLE_ITEM_BASE[0]);

const ItemBase *
ItemBase::item_base_get_pointer_from_id(size_t const id)
{
	assert( id < ITEM_BASE_COUNT );
	return &(TABLE_ITEM_BASE[id]);
}


struct ItemEntity;
typedef struct ItemEntity * pointer_weapon;

enum itementity_stat_type {
	itementity_stat_quality ,
	itementity_stat_durability_max ,
	itementity_stat_durability_current ,
	ITEMENTITY_STAT_COUNT ,
};



const char * STRINGTABLE_itementity_STAT[ITEMENTITY_STAT_COUNT] = {
	[itementity_stat_quality] = "quality" ,
	[itementity_stat_durability_max] = "durability_max" ,
	[itementity_stat_durability_current] = "durability_current" ,
};






void
display_table_item_type(FILE * f)
{
	for(int i = 0; i < COUNT_ITEM_TYPE; ++i) {
		fprintf(f , "%d %s\n" , i , STRINGTABLE_ITEM_TYPE[i] );
	}
}


void
ItemBase::fprint( FILE * f
) const {
	fprintf( f , "{t %d rs %d rd %d rw %d"
			, type
			, required_stat[stat_type_strength]
			, required_stat[stat_type_dexterity]
			, required_stat[stat_type_wisdom] );
	/* weapon */
	fprintf( f , "rng %d dmg %d th %d "
			, range
			, rollmod[rollmod_type_damage]
			, rollmod[rollmod_type_to_hit] );
	if( name ) {
		fprintf( f , "%s}", name );
	}
}


void
display_table_item_base(FILE * f)
{
	for(size_t i = 0; i < ITEM_BASE_COUNT ; ++i ) {
		fprintf(f , "%lu" , i );
		TABLE_ITEM_BASE[i].fprint(f);
		fprintf(f , "\n" );
	}
}





struct ItemEntity {
	size_t item_base_id = 0;
	bool is_breakable = true;
	int stat[ITEMENTITY_STAT_COUNT] = {
		[itementity_stat_quality] = 0 ,
		[itementity_stat_durability_max] = 1 ,
		[itementity_stat_durability_current] = 1 ,
	};
	int get_rollmod(enum rollmod_type const rt);
	void fprint(FILE * f);
	void fprint_stat(FILE * f);
};


void
ItemEntity::fprint(FILE * f) {
	fprintf( f , "ItemEntity:{q %d,d %d/%d,base[0x%lx]:{"
			,stat[itementity_stat_quality]
			,stat[itementity_stat_durability_max]
			,stat[itementity_stat_durability_current]
			,item_base_id
		   );
	const struct ItemBase * ptr_weaponbase = ItemBase::item_base_get_pointer_from_id(item_base_id);
	ptr_weaponbase->fprint(f);
	fprintf( f , "}}" );
}


int
ItemEntity::get_rollmod(enum rollmod_type const rt) {
	if( stat[itementity_stat_durability_current] <= 0 ) {
		return 0;
	}
	int const rollmod = TABLE_ITEM_BASE[item_base_id].get_rollmod(rt);
	return (rollmod + stat[itementity_stat_quality]);
}




enum ability_type {
	ability_type_none ,
	ability_type_attack , // governed by the type of weapon, so the range(if I get to implement range) will be set to weapon's range
	ability_type_instant ,
	ability_type_magic ,
	ability_type_buff ,
	ability_type_apply_counters ,
	ABILITY_TYPE_COUNT ,
};

const char *
STRINGTABLE_ABILITY_TYPE[ABILITY_TYPE_COUNT] = {
	[ability_type_none] = "[[ability_type_none]]" ,
	[ability_type_attack] = "attack" ,
	[ability_type_instant] = "instant" ,
	[ability_type_magic] = "magic" ,
	[ability_type_buff] = "buff" ,
	[ability_type_apply_counters] = "apply_counters" ,
};

const char *
STRINGTABLE_IS_ADD_COUNTER[ABILITY_TYPE_COUNT] = {
	[false] = "-" ,
	[true]  = "+" ,
};


struct AbilityResult {
	enum counter_type counter_type;
	int count;
	struct RollResult roll_result;
	/* bool is_add_counter = false; /1* delete this *1/ */
	AbilityResult(
			 bool _is_add_counter
			,struct RollResult _roll_result
			,enum counter_type _counter_type) {
		roll_result = _roll_result;
		counter_type = _counter_type;
		auto const success_level = roll_result.get_success_level();
		count =
			  _is_add_counter
			?   success_level
			: (-success_level);
	}
	void fprint( FILE * f ) const;
	int get_counter_delta() const;
};


int
AbilityResult::get_counter_delta() const {
	return count;
}


void
AbilityResult::fprint( FILE * f ) const {
	assert( counter_type < (int)TARGETING_TYPE_COUNT );
	assert( counter_type > 0 );
	fprintf( f , "(AbilityResult %s %d ; "
			, STRINGTABLE_COUNTERTYPE_SYMBOL[counter_type]
			, count );
	roll_result.fprint(f);
	fprintf( f , ")" );
	// roll_result;
}

// idea: `vector_counter_type` and `vector_rollmod`
struct Ability {
	const char * name = "[[ability.name:none]]";
	enum ability_type type = ability_type_none;
	enum counter_type on_success_counter_type = counter_type_none;
	bool is_add_counter = true;
	enum targeting_type targeting_type = targeting_type_none;
	enum rollmod_type rollmod_type = rollmod_type_none;
	int rollmod_add = 0;
	int rollmod_multiply = 0;
	int range = 1;
	void fprint(FILE * f);

	struct AbilityResult
		make_roll_result(int const score_add , int const score_multiply) const;
};


typedef std::vector<Ability *> VectorAbilityPointers;


enum ability_id {
	ability_none ,
	ability_attack ,
	ability_open_wounds ,
	ability_weaken ,
	ABILITY_COUNT ,
};

void 
Ability::fprint(FILE * f) {
	fprintf( f , "(%s%s|%6s|%6s) TH %3d MULT %3d %s"
			,STRINGTABLE_IS_ADD_COUNTER[is_add_counter]
			,STRINGTABLE_COUNTERTYPE_SYMBOL[on_success_counter_type]
			,STRINGTABLE_ABILITY_TYPE[type]
			,STRINGTABLE_TARGETING_TYPE[targeting_type]
			,rollmod_add
			,rollmod_multiply
			,name
		   );
}





Ability ARRAY_ABILITIES[] = {
	{
		.name = "Attack" ,
		.type = ability_type_attack ,
		.on_success_counter_type = counter_type_damage ,
		.targeting_type = targeting_type_enemy ,
		.rollmod_type = rollmod_type_attack ,
		.rollmod_add = 0 ,
		.rollmod_multiply = 1 ,
		.range = 1
	} ,
	{
		.name = "Open Wounds" ,
		.type = ability_type_attack ,
		.on_success_counter_type = counter_type_bleed ,
		.targeting_type = targeting_type_enemy ,
		.rollmod_type = rollmod_type_attack ,
		.rollmod_add = -4 ,
		.rollmod_multiply = -2 ,
		.range = 1
	} ,
	{
		.name = "Heal" ,
		.type = ability_type_buff ,
		.on_success_counter_type = counter_type_damage ,
		.is_add_counter = false ,
		.targeting_type = targeting_type_self,
		.rollmod_type = rollmod_type_magic ,
		.rollmod_add = -3 ,
		.rollmod_multiply = 2 ,
		.range = 1
	} ,
	{
		.name = "Weaken" ,
		.type = ability_type_magic ,
		.on_success_counter_type = counter_type_weakness ,
		.targeting_type = targeting_type_enemy ,
		.rollmod_type = rollmod_type_magic ,
		.rollmod_add = -3 ,
		.rollmod_multiply = 2 ,
		.range = 1
	} ,
	{
		.name = "Induce Poison" ,
		.type = ability_type_magic ,
		.on_success_counter_type = counter_type_poison ,
		.targeting_type = targeting_type_enemy ,
		.rollmod_type = rollmod_type_magic ,
		.rollmod_add = -5 ,
		.rollmod_multiply = 1 ,
		.range = 1
	} ,
};

const size_t SIZEOF_TABLE_ABILITY = sizeof(ARRAY_ABILITIES) / sizeof(ARRAY_ABILITIES[0]);


void
CHECK_TABLE_ABILITY(void) {
	for( auto &a : ARRAY_ABILITIES ) {
		if( a.type == ability_type_none ) {
			fprintf( stderr , "ability_type_none at ability \"%s\"\n"
					,a.name );
		}
	}
}

void print_table_ability(FILE * f) {
	size_t i = 0;
	for( auto &a : ARRAY_ABILITIES ) {
		fprintf( f
				, "0x%zx"
				, i );
		a.fprint(f);
		fprintf(f,"\n");
		++i;
	}
}



AbilityResult
Ability::make_roll_result(int const score_add , int const score_multiply) const {
	auto const roll_result
		= RollResult(
				 score_add + rollmod_add
				,score_multiply + rollmod_multiply);
	auto const ability_result =
		AbilityResult(
		 is_add_counter
		,roll_result
		,on_success_counter_type);

	return ability_result;
}


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



struct EffectEntity {
	const char * name = "[[effectentity:noname]]";
	int duration = 1;
	int rollmod[ROLLMOD_TYPE_COUNT] = { 0 };
	void fprint(FILE * f) const ;
};

void
EffectEntity:: fprint(FILE * f) const {
	fprintf( f , "{effect '%s' , duration %d ,  " , name , duration);
	fprint_rollmod_array( f , rollmod );
	fprintf( f , "}");
}



struct CombatEntity {
	std::array< int , COUNTER_TYPE_COUNT > counter_array = {0};
	std::array< int , STAT_TYPE_COUNT > max_stat = {{
		[stat_type_none] = 0 ,
		[stat_type_strength] = 1 ,
		[stat_type_dexterity] = 1 ,
		[stat_type_wisdom] = 1 ,
	}};
	std::array< int , STAT_TYPE_COUNT > arr_stat = {{
		[stat_type_none] = 0 ,
		[stat_type_strength] = 1 ,
		[stat_type_dexterity] = 1 ,
		[stat_type_wisdom] = 1 ,
	}};

	VectorAbilityPointers vector_available_abilities = VECTOR_ABILITY_POINTERS_DEFAULT;
	std::vector< const char * >
		vector_available_abilities_strings
		= vector_ability_pointer_get_vector_of_strings(
				vector_available_abilities );
	Ability * ptr_available_ability( int const id ) const;

	//methods
	void fprint(FILE * f);
	void fprint_long(FILE * f);
	void fprint_hp(FILE * f);
	void fprint_all_counters(FILE * f);
	void fprint_nonzero_counters(FILE * f);

	bool is_dead() const {
		return (
				get_counter_value(counter_type_damage)
				>
				get_rollmod(rollmod_type_vitality)
			   );
	};
	bool is_alive() const {
		return !is_dead();
	};

	int get_counter_value(enum counter_type ct) const;
	int * ptr_counter(enum counter_type ct);
	int get_rollmod(enum rollmod_type const rt) const;
	int get_stat(enum stat_type const st) const;
	int get_max_stat(enum stat_type const st) const;

	void receive_damage(int damage);
	void apply_ability_result( const AbilityResult &ability_result );

	void perform_post_counters_per_round_effects(void);

	void equip(); //TODO
};



int CombatEntity::get_rollmod(enum rollmod_type const rt) const {
	 /* TODO make use of this */
	int rollmod = 0;
	switch(rt) {
		case rollmod_type_strength:
			rollmod =
				(get_stat(stat_type_strength)
				 - get_counter_value(counter_type_weakness));
			break;
		case rollmod_type_dexterity:
			rollmod =
				(get_stat(stat_type_dexterity)
				 - get_counter_value(counter_type_slowness));
			break;
		case rollmod_type_wisdom:
			rollmod = get_stat(stat_type_wisdom);
			break;
		case rollmod_type_magic:
			rollmod = get_rollmod(rollmod_type_wisdom);
			break;
		case rollmod_type_attack:
		case rollmod_type_to_hit:
			rollmod = get_rollmod(rollmod_type_dexterity);
			break;
		case rollmod_type_damage:
			rollmod = get_rollmod(rollmod_type_strength);
			break;
		case rollmod_type_defense:
			rollmod = get_rollmod(rollmod_type_dexterity);
			break;
		case rollmod_type_vitality:
			rollmod =
				VITALITY_BASE
				+ (STRENGTH_MULTIPLIER_VITALITY
						* get_rollmod(rollmod_type_strength));
			if( rollmod < VITALITY_MINIMUM ) {
				rollmod = VITALITY_MINIMUM;
			}
			break;
		default:
			fprintf( stderr , "get_rollmod:unexpected rollmod_type = %d" , rt );
	}
	return rollmod;
}


int
CombatEntity::get_stat(enum stat_type const st) const {
	assert( st > stat_type_none );
	assert( st < STAT_TYPE_COUNT );
	return arr_stat[st];
}


int
CombatEntity::get_max_stat(enum stat_type const st) const {
	assert( st > stat_type_none );
	assert( st < STAT_TYPE_COUNT );
	return max_stat[st];
}


RollResult roll_attack(
		 const CombatEntity & attacker
		,const CombatEntity & target
		) {
	int const rollmod_attack = attacker.get_rollmod(rollmod_type_to_hit);
	int const rollmod_defense = target.get_rollmod(rollmod_type_defense);
	int const rollmod_damage = attacker.get_rollmod(rollmod_type_damage);
	return RollResult(
			(rollmod_attack - rollmod_defense)
			,rollmod_damage ) ;
}


Ability *
CombatEntity::ptr_available_ability(
		int const id
		) const {
	assert( id >= 0 );
	assert( id < (int)vector_available_abilities.size() );
	Ability * ref = vector_available_abilities[id];
	return ref;
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
	fprint_hp(f);
	fprintf( f , "STR %d , DEX %d , WIS %d ;"
			,get_stat(stat_type_strength)
			,get_stat(stat_type_dexterity)
			,get_stat(stat_type_wisdom)
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
	fprintf( f , "(VIT: %d)"
			,get_rollmod(rollmod_type_vitality)
			);
}


void CombatEntity::receive_damage(int damage) {
	if( damage < 0 ) {
		damage = 0;
	}
	(*ptr_counter(counter_type_damage)) += damage;
}


void
CombatEntity::apply_ability_result(
		const AbilityResult &ability_result
		) {
	auto ptr = ptr_counter(ability_result.counter_type);
	(*ptr) += ability_result.get_counter_delta();
	// TODO
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


int * CombatEntity::ptr_counter(enum counter_type ct) {
	assert( ct > counter_type_none );
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
	receive_damage( counter_perform_halving(ptr_counter(counter_type_poison)) );
	receive_damage( counter_perform_halving(ptr_counter(counter_type_bleed)) );
	// only reduce
	counter_perform_decrement(ptr_counter(counter_type_slowness));
	counter_perform_decrement(ptr_counter(counter_type_weakness));
}



AbilityResult
roll_ability_result(
		 const Ability * ptr_ability
		,const CombatEntity &actor
		,const CombatEntity &target) {
	/* TODO handling different types of stats for different abilities */
	int bonus_actor = 0;
	int bonus_target = 0;
	int score_multiply = ptr_ability->rollmod_multiply;
	switch(ptr_ability->type) {
		case ability_type_attack:
			bonus_actor = actor.get_rollmod(rollmod_type_to_hit);
			bonus_target = target.get_rollmod(rollmod_type_defense);
			score_multiply += actor.get_rollmod(rollmod_type_damage);
			break;
		case ability_type_magic:
			bonus_actor = actor.get_rollmod(rollmod_type_magic);
			break;
		case ability_type_buff:
			bonus_actor = actor.get_rollmod(rollmod_type_magic);
			break;
		default:
			fprintf( stderr
					,"roll_ability_result cannot handle ability_type %d"
					,ptr_ability->type );
			break;
	}
	return ptr_ability->make_roll_result(
			 (bonus_actor - bonus_target)
			,score_multiply);
}



void
apply_result_in_combat(
		 CombatEntity * target
		,const AbilityResult ability_result
		) {
	assert( target );
	target->apply_ability_result( ability_result );
};


CombatEntity *
get_target_of_ability( /* TODO handle more entities than actor-enemy, but that will come only after I make a robust system for selection */
		 CombatEntity &actor
		,CombatEntity &enemy
		,const Ability * chosen_ability
		) {
	assert( chosen_ability );
	CombatEntity * target;
	switch(chosen_ability->targeting_type) {
		case targeting_type_self:
			target = &actor;
			break;
		case targeting_type_enemy:
			target = &enemy;
			break;
		default:
			fprintf(  stderr
					,"unexpected targeting_type=%d\n"
					,chosen_ability->targeting_type );
	}
	return target;
};


void
apply_results_in_combat(
		 CombatEntity * actor
		,CombatEntity * enemy
		,const Ability * chosen_ability
		,const AbilityResult ability_result
		) {
	assert( actor );
	assert( enemy );
	assert( chosen_ability );
	switch(chosen_ability->targeting_type) {
		case targeting_type_self:
			actor->apply_ability_result( ability_result );
			break;
		case targeting_type_enemy:
			enemy->apply_ability_result( ability_result );
			break;
		default:
			fprintf(  stderr
					,"unexpected targeting_type=%d\n"
					,chosen_ability->targeting_type );
	}
};








void
perform_example_combat(FILE * f)
{
	CHECK_TABLE_ABILITY();
	print_table_ability(f);
	struct CombatEntity you;
	you.arr_stat[stat_type_strength] = 3;
	you.arr_stat[stat_type_dexterity] = 4;
	struct CombatEntity foe;

	fprintf( f , "you: " );
	you.fprint(f);
	fprintf( f , "\n" );
	fprintf( f , "foe: " );
	foe.fprint(f);
	fprintf( f , "\n" );
	int const ROUND_COUNT = 8;

	int round = 0;
	for( ; round < ROUND_COUNT ; ++round ) {
		fprintf(f , "  round %d\n" , round);
		int selection = select_fprint_vector_of_strings( stdout , you.vector_available_abilities_strings ); //TODO
		if( selection < 0 || selection >= (int)you.vector_available_abilities.size() ) { // I'm (probably unneccesarily) afraid of casting here
			fprintf( f , "aborting, since you selected %d\n" , selection);
			goto jump_end;
		}
		fprintf( f, "you(%d):" , selection);
		const auto you_ability = you.ptr_available_ability(selection);
		const AbilityResult you_ability_result
			= roll_ability_result(
					 you_ability
					,you
					,foe );
		you_ability_result.fprint( f );
		fprintf( f , "\n" );
		int const enemy_selection = rand() % 3;
		fprintf( f, "foe(%d):" , enemy_selection );
		const auto foe_ability = foe.ptr_available_ability(selection);
		const AbilityResult foe_ability_result
			= roll_ability_result(
					 foe_ability
					,foe
					,you );
		foe_ability_result.fprint( f );
		fprintf( f , "\n" );
		auto you_targeted_entity = get_target_of_ability(
				 you
				,foe
				,you_ability);
		auto foe_targeted_entity = get_target_of_ability(
				 foe
				,you
				,foe_ability);
		apply_result_in_combat(
				 you_targeted_entity
				,you_ability_result );
		apply_result_in_combat(
				 foe_targeted_entity
				,foe_ability_result );
		if( you.is_dead() ) {
			fprintf( f , "you died\n" );
			break;
		}
		if( foe.is_dead() ) {
			fprintf( f , "foe died\n" );
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


