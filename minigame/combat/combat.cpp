#include "combat.h"

int const ROUND_COUNT = 8;


enum material_type {
	material_type_none ,
	material_type_food ,
	material_type_wood ,
	material_type_iron ,
	material_type_coal ,
	material_type_rope ,
	material_type_fabric ,
	material_type_solvent ,
	COUNT_MATERIAL_TYPE ,
};

typedef
std::array< int , COUNT_MATERIAL_TYPE>
ArrayMaterials;



enum item_type {
	item_type_none ,
	item_type_zero ,
	item_type_wearable ,
	item_type_consumable ,
	item_type_permanent ,
	item_type_sword ,
	item_type_axe ,
	item_type_polearm ,
	item_type_club ,
	item_type_throwable ,
	item_type_ranged_thrower ,
	item_type_junk ,
	item_type_junk_wood ,
	item_type_junk_iron ,
	item_type_junk_coal ,
	item_type_junk_rope ,
	item_type_junk_fabric ,
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
	[item_type_zero]
		= "item_type_zero" ,
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
	[item_type_junk]
		= "item_type_junk" ,
};


enum slot_type {
	slot_type_none ,
	slot_type_hand_main ,
	slot_type_hand_second ,
	slot_type_head ,
	slot_type_body ,
	slot_type_legs ,
	slot_type_ring ,
	COUNT_SLOT_TYPE ,
};


std::array<const char * , COUNT_SLOT_TYPE>
STRINGTABLE_SLOT_TYPE = {{
	[slot_type_none]
		= "slot_type_none" ,
	[slot_type_hand_main]
		= "slot_type_hand_main" ,
	[slot_type_hand_second]
		= "slot_type_hand_second" ,
	[slot_type_head]
		= "slot_type_head" ,
	[slot_type_body]
		= "slot_type_body" ,
	[slot_type_legs]
		= "slot_type_legs" ,
	[slot_type_ring]
		= "slot_type_ring" ,
}};



enum monster_type {
	monster_type_none ,
	monster_type_devil ,
	monster_type_blob ,
	monster_type_porkish ,
	monster_type_mootant ,
	monster_type_undead ,
	monster_type_vermin ,
	MONSTER_TYPE_COUNT ,
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

std::array<const MonsterType , MONSTER_TYPE_COUNT>
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
	counter_type_resolve ,
	counter_type_damage , // this will be "missing health", and replaces "hp_current"
	counter_type_poison ,
	counter_type_bleed ,
	counter_type_modify_stat_dexterity ,
	counter_type_modify_stat_strength ,
	counter_type_modify_stat_wisdom ,
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
	[counter_type_resolve] = "HR" ,
	[counter_type_damage] = "RD",
	[counter_type_poison] = "cP" ,
	[counter_type_bleed] = "cB" ,
	[counter_type_modify_stat_dexterity] = "mD" ,
	[counter_type_modify_stat_strength] = "mS" ,
	[counter_type_modify_stat_wisdom] = "mW" ,
}};


std::array<const char * , COUNTER_TYPE_COUNT>
STRINGTABLE_COUNTERTYPE_NAME_POSITIVE {{
	[counter_type_none] = "[[counter_type_none]]" ,
	[counter_type_resolve] = "HeroicResolve" ,
	[counter_type_damage] = "ReceivedDamage",
	[counter_type_poison] = "Poison" ,
	[counter_type_bleed] = "Bleed" ,
	[counter_type_modify_stat_dexterity] = "Quickness" ,
	[counter_type_modify_stat_strength] = "Might" ,
	[counter_type_modify_stat_wisdom] = "Enlightnment" ,
}};


std::array<const char * , COUNTER_TYPE_COUNT>
STRINGTABLE_COUNTERTYPE_NAME_NEGATIVE = {{
	[counter_type_none] = "[[counter_type_none]]" ,
	[counter_type_resolve] = "Despair" ,
	[counter_type_damage] = "Overheal",
	[counter_type_poison] = "Antibodies(resist poison)" ,
	[counter_type_bleed] = "Clotting(resist bleed)" ,
	[counter_type_modify_stat_dexterity] = "Slowness" ,
	[counter_type_modify_stat_strength] = "Weakness" ,
	[counter_type_modify_stat_wisdom] = "Dumbness" ,
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
		? STRINGTABLE_COUNTERTYPE_NAME_POSITIVE[ct]
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
	enum slot_type slot_type = slot_type_none;
	bool is_2handed = false;
	bool is_breakable = true;
	int default_max_durability = 1;
	int required_stat[STAT_TYPE_COUNT] = { 0 };
	// itemstat_type]
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
		type  : item_type_zero,
		range : 1,
		name  :  "[[BUG? item zero should not be seen in game! something gone wrong!]]" 
	},

	{
		type : item_type_wearable ,
		slot_type : slot_type_hand_main ,
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
		type : item_type_wearable ,
		slot_type : slot_type_hand_main ,
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
		type : item_type_wearable ,
		slot_type : slot_type_hand_main ,
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
		type : item_type_wearable ,
		slot_type : slot_type_hand_main ,
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
		type : item_type_wearable ,
		slot_type : slot_type_hand_main ,
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

	{
		type : item_type_wearable ,
		slot_type : slot_type_body ,
		default_max_durability : 12 ,
		rollmod  : {
			[rollmod_type_none     ]  = 0 ,
			[rollmod_type_vitality ]  = 0 ,
			[rollmod_type_attack   ]  = 0 ,
			[rollmod_type_magic    ]  = 0 ,
			[rollmod_type_to_hit   ]  = 0 ,
			[rollmod_type_defense  ]  =  3 ,
			[rollmod_type_damage   ]  = 0 ,
			[rollmod_type_strength ]  = 0 ,
			[rollmod_type_dexterity]  = 0 ,
			[rollmod_type_wisdom   ]  = 0 ,
		} ,
		name  :  "Gambeson" 
	},

	{
		type : item_type_wearable ,
		slot_type : slot_type_body ,
		default_max_durability : 20 ,
		rollmod  : {
			[rollmod_type_none     ]  = 0 ,
			[rollmod_type_vitality ]  = 0 ,
			[rollmod_type_attack   ]  = 0 ,
			[rollmod_type_magic    ]  = 0 ,
			[rollmod_type_to_hit   ]  = 0 ,
			[rollmod_type_defense  ]  =  6 ,
			[rollmod_type_damage   ]  = 0 ,
			[rollmod_type_strength ]  = 0 ,
			[rollmod_type_dexterity]  = 0 ,
			[rollmod_type_wisdom   ]  = 0 ,
		} ,
		name  :  "Hauberk" 
	},

/* 	{ */
/* 		type : item_type_axe , */
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


int const COUNT_ITEM_BASE = sizeof(TABLE_ITEM_BASE) / sizeof(TABLE_ITEM_BASE[0]);

const ItemBase *
ItemBase::item_base_get_pointer_from_id(size_t const id)
{
	assert( id < COUNT_ITEM_BASE );
	return &(TABLE_ITEM_BASE[id]);
}


void
fprint_table_item_base_name_only( FILE * f ) {
	size_t n = 0;
	for( auto const &ib : TABLE_ITEM_BASE ) {
		fprintf( f, "0x%02zx \"%s\"\n"
				,n
				,ib.name
				);
		++n;
	}
}


struct ItemEntity;
typedef struct ItemEntity * pointer_weapon;

enum itemstat_type {
	itemstat_quality ,
	itemstat_durability_max ,
	itemstat_durability_current ,
	COUNT_ITEMSTAT ,
};





const char * STRINGTABLE_ITEMENTITY_STAT[COUNT_ITEMSTAT] = {
	[itemstat_quality] = "quality" ,
	[itemstat_durability_max] = "durability_max" ,
	[itemstat_durability_current] = "durability_current" ,
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
	fprintf( f , "{t %d rs %d rd %d rw %d " /* TODO print all stats */
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
	for(size_t i = 0; i < COUNT_ITEM_BASE ; ++i ) {
		fprintf(f , "%lu" , i );
		TABLE_ITEM_BASE[i].fprint(f);
		fprintf(f , "\n" );
	}
}





struct ItemEntity {
	size_t item_base_id = 0; /* TODO add ref_itembase() */
	/* maybe even remove the concept of item_base_id and only use reference to itembase? */
	bool is_breakable = true;
	int stat[COUNT_ITEMSTAT] = {
		[itemstat_quality] = 0 ,
		[itemstat_durability_max] = 1 ,
		[itemstat_durability_current] = 1 ,
	};
	enum item_type get_item_type(void) const {
		return TABLE_ITEM_BASE[item_base_id].type;
	}
	bool is_none( void ) const {
		return item_base_id == 0;
	}
	bool is_2handed( void ) const {
		return TABLE_ITEM_BASE[item_base_id].is_2handed;
	}
	int get_rollmod(enum rollmod_type const rt) const ;
	enum slot_type get_slottype(void) const {
		return TABLE_ITEM_BASE[item_base_id].slot_type;
	}
	int get_required_stat(enum stat_type const st) const {
		return TABLE_ITEM_BASE[item_base_id].required_stat[st];
	}
	void fprint(FILE * f) const;
	void fprint_stat(FILE * f) const;

	ItemEntity( ) {  } ;

	ItemEntity(
			 const size_t _item_base_id
			,int const quality
			,int const durability_delta
			) {
		assert(_item_base_id < COUNT_ITEM_BASE);
		item_base_id = _item_base_id;
		stat[itemstat_quality] = quality;
		stat[itemstat_durability_max]
			= TABLE_ITEM_BASE[_item_base_id].default_max_durability ;
		stat[itemstat_durability_current]
			= stat[itemstat_durability_max]
			+ durability_delta ;
	}
};


void
ItemEntity::fprint(FILE * f) const {
	fprintf( f , "{q %d,d %d/%d,base[0x%lx]:"
			,stat[itemstat_quality]
			,stat[itemstat_durability_max]
			,stat[itemstat_durability_current]
			,item_base_id
		   );
	const struct ItemBase * ptr_weaponbase = ItemBase::item_base_get_pointer_from_id(item_base_id);
	ptr_weaponbase->fprint(f);
	fprintf( f , "}" );
}


int
ItemEntity::get_rollmod(enum rollmod_type const rt) const {
	if( stat[itemstat_durability_current] <= 0 ) {
		return 0;
	}
	int const rollmod = TABLE_ITEM_BASE[item_base_id].get_rollmod(rt);
	int const quality_bonus
		= rollmod > 0
		? stat[itemstat_quality]
		: 0;
	return (rollmod + quality_bonus);
}


typedef std::array<ItemEntity , COUNT_SLOT_TYPE>
EquippedItem; /* seems like it could allow duping, but whatever, we will see */
/* it could be more robust to have a global vector of item entities in game, and then just pass references/pointers around */


void
fprint_equipped_items(
		 FILE * f
		,EquippedItem const &eq )
{
	int equipped_items = 0;
	for( const auto &e : eq) {
		if( !(e.is_none()) ) {
			fprintf( f , "%s:"
					,STRINGTABLE_SLOT_TYPE[e.get_slottype()] );
			e.fprint(f);
			fprintf( f , "\n" );
			++equipped_items;
		}
	}
	fprintf( f , "equipped_items count: %d" , equipped_items );
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
	int get_counter_delta() const; /* the hell was that supposed to be?/ */
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
	void fprint(FILE * f) const;

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
Ability::fprint(FILE * f) const {
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





Ability TABLE_ABILITIES[] = {
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
		.on_success_counter_type = counter_type_modify_stat_strength ,
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

const size_t ABILITIES_COUNT = sizeof(TABLE_ABILITIES) / sizeof(TABLE_ABILITIES[0]);


void
CHECK_TABLE_ABILITY(void) {
	bool found_errors = false;
	for( auto &a : TABLE_ABILITIES ) {
		if( a.type == ability_type_none ) {
			fprintf( stderr , "ability_type_none at ability \"%s\"\n"
					,a.name );
			found_errors = true;
		}
	}
	if( found_errors ) {
		fprintf( stderr , "found errors when checking TABLE_ABILITIES\n" );
	}
}


void
CHECK_TABLE_ITEM_BASE(void) {
	/* TODO check for validity of all type-signifying enums */
	bool found_errors = false;
	for( auto &ib : TABLE_ITEM_BASE ) {
		bool has_errors = false;
		/* don't allow item_none */
		if( ib.type == item_type_none ) {
			has_errors = true;
			fprintf( stderr , "(item is of item_type_none)" );
			ib.fprint(stderr) ;
		}
		/* wearable items must have set slot_type */
		if( ib.type == item_type_wearable && ib.slot_type == slot_type_none ) {
			has_errors = true;
			fprintf( stderr , "(wearable item without proper slot_type)" );
		}
		/* 2handed weapons must have slot_type_hand_main */
		if( ib.is_2handed
				&&
				(ib.slot_type != slot_type_hand_main)
				 ) {
			has_errors = true;
			fprintf( stderr , "(2handed weapon with slot_type != %d: \"%s\")"
				,slot_type_hand_second
				,STRINGTABLE_SLOT_TYPE[slot_type_hand_second] );
		}
		if(has_errors) {
			has_errors = true;
			ib.fprint(stderr) ;
			fprintf( stderr , "\n");
			found_errors = true;
		}
	}

	if( found_errors ) {
		fprintf( stderr , "found errors when checking TABLE_ITEM_BASE\n" );
	}
}



void print_table_ability(FILE * f) {
	size_t i = 0;
	for( auto &a : TABLE_ABILITIES ) {
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
	&(TABLE_ABILITIES[0]) ,
	&(TABLE_ABILITIES[1]) ,
	&(TABLE_ABILITIES[2]) ,
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
	bool is_alive = true;
	std::array< int , COUNTER_TYPE_COUNT > counter = {0};
	std::array< int , STAT_TYPE_COUNT > max_stat = {{
		[stat_type_none] = 0 ,
		[stat_type_level] = MAX_LEVEl ,
		[stat_type_strength] = 1 ,
		[stat_type_dexterity] = 1 ,
		[stat_type_wisdom] = 1 ,
	}};
	std::array< int , STAT_TYPE_COUNT > stat = {{
		[stat_type_none] = 0 ,
		[stat_type_level] = 0 ,
		[stat_type_strength] = 1 ,
		[stat_type_dexterity] = 1 ,
		[stat_type_wisdom] = 1 ,
	}};
	
	std::array<int , ROLLMOD_TYPE_COUNT >
		equipment_rollmod;
	std::array<bool , ABILITIES_COUNT >  /* hmm - just noticed, that it will be slightly more difficult to program rudimentary AI(i will need to implement probability_weight for abilities anyway). for now, the AI will only have 1 ability */
		arr_is_ability_available = {{ true, false }};

	EquippedItem equipped;

	VectorAbilityPointers vector_available_abilities = VECTOR_ABILITY_POINTERS_DEFAULT;
	std::vector< const char * >
		vector_available_abilities_strings
		= vector_ability_pointer_get_vector_of_strings(
				vector_available_abilities );
	Ability * ptr_available_ability( int const id ) const;

	//methods
	void fprint(FILE * f) const;
	void fprint_long(FILE * f) const;
	void fprint_hp(FILE * f) const;
	void fprint_all_counters(FILE * f) const;
	void fprint_nonzero_counters(FILE * f) const;

	bool is_slot_empty( enum slot_type const slot_type ) const {
		assert( slot_type > slot_type_none );
		assert( slot_type < COUNT_SLOT_TYPE );
		return equipped[slot_type].is_none();
	}

	bool is_ability_available( size_t const ability_id ) const {
		if(ability_id >= ABILITY_COUNT) {
			return false;
		}
		return arr_is_ability_available.at(ability_id);
	}
	std::vector<const char * >
		generate_vector_of_available_ability_names(void) const;


	int get_counter_value(enum counter_type ct) const;
	int * ptr_counter(enum counter_type ct);
	int get_rollmod(enum rollmod_type const rt) const;
	int get_itembonus_rollmod(enum rollmod_type const rt) const;
	int get_stat(enum stat_type const st) const;
	int get_max_stat(enum stat_type const st) const;

	int receive_damage(int damage); /*** return dealt damage */
	void apply_ability_result( const AbilityResult &ability_result );

	void perform_post_round_calculations(void);

	bool equip(
			/* return true on success */
			 FILE * f /* would be nice to decouple interface */
			,ItemEntity &ie /* change this itementity into default itementity(that is: none)*/
			);

	void fprint_equipped( FILE * f ) const {
		fprint_equipped_items(f , equipped);
	}
	void fprint_rollmods_equipped( FILE * f ) const;
	void fprint_rollmods_equipped_only_nonzero( FILE * f ) const ;
	void fprint_available_abilities(FILE * f) const;

	void calculate_equipment_bonuses(void);
};

void
CombatEntity::calculate_equipment_bonuses(void) {
	equipment_rollmod = { 0 };

	for(auto const &ie : equipped ) {
		if( ie.get_item_type() == item_type_wearable  ) {
			for( int rt = (rollmod_type_none + 1) /* this iterating disturbs me */
					; rt < ROLLMOD_TYPE_COUNT /* but alas - no better idea currently */
					; ++rt ) {
				auto const cur_rt = static_cast< enum rollmod_type >(rt);
				equipment_rollmod[rt] += ie.get_rollmod(cur_rt);
			}
		}
	}
}


bool
CombatEntity::equip(
		 FILE * f /* would be nice to decouple interface */
		,ItemEntity &ie
		) {
	if( ie.is_none() ) {
		fprintf( f , "tried to equip item that is_none\n"); 
		return false;
	}
	if( ie.get_item_type() != item_type_wearable ) {
		fprintf( f , "tried to equip item that is not wearable\n"); 
		return false;
	}

	for( int stat_type = (stat_type_none + 1) /* this iterating disturbs me */
			; stat_type < STAT_TYPE_COUNT /* but alas - no better idea currently */
			; ++stat_type ) {
		auto const cur_stat_type   = static_cast< enum stat_type >(stat_type);
		int const has_stat = get_stat(cur_stat_type);
		int const need_stat = ie.get_required_stat(cur_stat_type);
		if( has_stat < need_stat ) {
			fprintf( f , "too low %s: (has: %d need: %d)\n"
					,STRINGTABLE_STATTYPE_SYMBOL[cur_stat_type]
					,has_stat
					,need_stat
					);
			return false;
		}
	}


	enum slot_type slot_type = ie.get_slottype();
	if( !(is_slot_empty(slot_type)) ) {
		fprintf( f, "slot_type %d(%s) already occupied\n"
				,slot_type
				,STRINGTABLE_SLOT_TYPE[slot_type]
				);
		return false;
	}
	if( ie.is_2handed() && !(is_slot_empty(slot_type_hand_second)) ) {
		fprintf( f, "2handed weapon, but offhand(slot_type %d(%s)) already occupied\n"
				,slot_type_hand_second
				,STRINGTABLE_SLOT_TYPE[slot_type_hand_second]
				);
		return false;
	}

	/* all good: we can equip the item now */
	if( is_slot_empty(slot_type) ) { /* completely unnecesary, but I will leave this condition in case */
		equipped[slot_type] = ie;
		ie = ItemEntity();
		return true;
	}

	fprintf( f, "somehow slot_type %d(%s) is already occupied\n"
			,slot_type
			,STRINGTABLE_SLOT_TYPE[slot_type]
		   );

	return false;
}




int
CombatEntity::get_itembonus_rollmod(
		enum rollmod_type const rt
		) const {
	int bonus = 0;
	for( const auto &ie : equipped ) {
		if( ie.get_item_type() == item_type_wearable ) {
			bonus += ie.get_rollmod(rt);
		}
	}
	return bonus;
}


int CombatEntity::get_rollmod(enum rollmod_type const rt) const {
	int rollmod = 0;
	switch(rt) {
		case rollmod_type_strength:
			rollmod =
				(get_stat(stat_type_strength)
				 + get_counter_value(counter_type_modify_stat_strength));
			break;
		case rollmod_type_dexterity:
			rollmod =
				(get_stat(stat_type_dexterity)
				 + get_counter_value(counter_type_modify_stat_dexterity));
			break;
		case rollmod_type_wisdom:
			rollmod =
				(get_stat(stat_type_wisdom))
				 + get_counter_value(counter_type_modify_stat_wisdom);
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
	return stat[st];
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
CombatEntity::fprint(FILE * f) const {
	fprint_hp(f);
	fprintf( f , "STR %d , DEX %d , WIS %d ;"
			,get_stat(stat_type_strength)
			,get_stat(stat_type_dexterity)
			,get_stat(stat_type_wisdom)
			);
	fprint_nonzero_counters(f);
}


void
CombatEntity::fprint_nonzero_counters(FILE * f)  const {
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
CombatEntity::fprint_all_counters(FILE * f)  const {
	for( int i = 0; i < COUNTER_TYPE_COUNT; ++i ) {
		fprintf( f , " c%s%d"
				, STRINGTABLE_COUNTERTYPE_SYMBOL[i]
				, get_counter_value( (enum counter_type) i ) );
	}
}


void
CombatEntity::fprint_hp(FILE * f) const {
	fprintf( f , "(VIT: %d)"
			,get_rollmod(rollmod_type_vitality)
			);
}


void
CombatEntity::fprint_rollmods_equipped( FILE * f ) const {
	for(  int rt = rollmod_type_none
		 ; rt < ROLLMOD_TYPE_COUNT
		 ; ++rt
		 ) {
		fprintf( f , "%s %d "
				,STRINGTABLE_ROLLMOD_SHORTNAME[rt]
				,equipment_rollmod[rt]
				);
	}
}


void
CombatEntity::fprint_available_abilities(
		FILE * f
		) const {
	size_t id = 0;
	for( auto const is_avail : arr_is_ability_available ) {
		if( is_avail ) {
			fprintf( f , "0x%zx %zu" , id , id);
			TABLE_ABILITIES[id].fprint(f);
			++id;
			fprintf( f , "\n");
		}
	}
}


void
CombatEntity::fprint_rollmods_equipped_only_nonzero( FILE * f ) const {
	for(  int rt = rollmod_type_none
		 ; rt < ROLLMOD_TYPE_COUNT
		 ; ++rt
		 ) {
		int const rm = equipment_rollmod[rt];
		if( rm != 0 ) {
			fprintf( f , "%s %d "
					,STRINGTABLE_ROLLMOD_SHORTNAME[rt]
					,rm
				   );
		}
	}
}


int CombatEntity::receive_damage(int damage) {
	/* turns out this just isn't used at all xDD because everything goes through apply_ability_result */
	if( damage <= 0 ) {
		return 0;
	}

	/* hmm - how to handle resolve? I wanted resolve to nullify any damage that goes above max_damage that would kill the warrior */
	/* but it would mean, that the warrior will want to reduce their max health  */
	/* I know! but it will be harder to read */
	/* actually, no, because I have to work with is_dead function :/ */
	int const max_damage = get_rollmod(rollmod_type_vitality);
	int const cur_damage = counter[counter_type_damage];
	int const new_damage = cur_damage + damage;
	int const resolve = counter[counter_type_resolve];
	counter[counter_type_damage] = new_damage;

	if( new_damage > max_damage ) {
		is_alive = false;
		if( resolve > 0 ) {
			/* printf( "resolve saved (dmg %d , res %d)\n" */
			/* 		, new_damage */
			/* 		, resolve ); */
			/*  --(counter[counter_type_resolve]); */
			is_alive = true;
		} else {
			/* printf( "killed! %d  %d" , new_damage , resolve ); */
		}
	}

	return damage;
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
counter_function_reduction counter_perform_towards_zero_by_one;
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

int counter_perform_towards_zero_by_one(int * counter) {
	if((*counter) > 0) {
		 --(*counter);
	} else if((*counter) < 0) {
		 ++(*counter);
	}
	return (*counter);

}


int * CombatEntity::ptr_counter(enum counter_type ct) {
	assert( ct > counter_type_none );
	assert( ct >= 0);
	assert( ct < COUNTER_TYPE_COUNT);
	return &(counter[ct]);
}


int CombatEntity::get_counter_value(enum counter_type ct) const {
	assert( ct >= 0);
	assert( ct < COUNTER_TYPE_COUNT);
	return counter[ct];
}


void
CombatEntity::perform_post_round_calculations(void) {
	/* idea that will make for an interesting mechanic: at the end of each round, all rollmods and counters and stats will be recalculated for each entity */
	/* This will have 2 effects: */
	/* 1. gameplay:
	 * "first move" will matter much, much less.
	 * with the current pending idea for handling abilities and combat, it will mean that the effects will get stacked in a particular order, but I'm also thinking about working it all more like a queue: each round a vector_pending_ability_results_to_apply will be iterated over, and then emptied. it could be done also as a stack, but nah I don't think so. */
	/* 2. code:
	 * all calculations will be stored in one place.
	 * my premature_optimization instinct says, that it will be "le more efficient XD" than the current idea of doing everything a la functional programming.
	 * currently, I'm planning to make it so the rollmods are always calculated on-demand */
	// damage-over-time
	int damage = 0;
	damage += counter_perform_halving(ptr_counter(counter_type_bleed));
	{ /* poison*/
		/* It seems more interesting, if poison has a slightly different mechanic: instead of halving, */
		/* this means, that poison damages you faster with low values, but doesn't scale */
		damage += counter_perform_decrement(ptr_counter(counter_type_poison)) ;
		damage += counter_perform_decrement(ptr_counter(counter_type_poison)) ;
	}

	// only reduce
	counter_perform_towards_zero_by_one(ptr_counter(counter_type_modify_stat_dexterity));
	counter_perform_towards_zero_by_one(ptr_counter(counter_type_modify_stat_strength));
	counter_perform_towards_zero_by_one(ptr_counter(counter_type_modify_stat_wisdom));

	// apply damage
	receive_damage(damage);

	/* check if still alive */

	int const max_damage = get_rollmod(rollmod_type_vitality);
	int const cur_damage = counter[counter_type_damage];
	int const resolve = counter[counter_type_resolve];

	if( cur_damage > max_damage ) {
		is_alive = false;
		if( resolve > 0 ) {
			printf( "resolve saved (dmg %d , res %d)\n"
					, cur_damage
					, resolve );
			 --(counter[counter_type_resolve]);
			is_alive = true;
		} else {
			printf( "killed! %d  %d" , cur_damage , resolve );
		}
	}


	calculate_equipment_bonuses();
}


std::vector<const char * >
CombatEntity::generate_vector_of_available_ability_names(void) const{
	/* this is defs inneficient in terms of computing power, */
	/* but incredibly easy to understand */
	std::vector<const char * > vec_names;
	vec_names.reserve(ABILITIES_COUNT);
	size_t id = 0;
	for( auto const b : arr_is_ability_available ) {
		if( b ) {
			vec_names.push_back( TABLE_ABILITIES[id].name );
		}
		++id;
	}
	return vec_names;
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


CombatEntity
generate_enemy_of_level(
		int const level
		)
{
	auto enemy = CombatEntity();
	enemy.stat[stat_type_strength]  += level;
	enemy.stat[stat_type_dexterity] += level;
	enemy.stat[stat_type_wisdom]    += level;
	
	enemy.max_stat[stat_type_strength]  += level;
	enemy.max_stat[stat_type_dexterity] += level;
	enemy.max_stat[stat_type_wisdom]    += level;
	return enemy;
}



struct PlayerEntity {
	ArrayMaterials material = { 0 };
	std::vector<CombatEntity> vector_warriors;


	PlayerEntity();
};


PlayerEntity::PlayerEntity() {
	CombatEntity first_warrior;
	first_warrior.counter[counter_type_resolve] = 2;
	vector_warriors.emplace_back(first_warrior);
}




void fight_versus_opponent(
		 FILE * f
		,CombatEntity &you
		,CombatEntity &foe
		)
{
	fprintf( f , "you: " );
	you.fprint(f);
	fprintf( f , "\n" );
	fprintf( f , "foe: " );
	foe.fprint(f);
	fprintf( f , "\n" );

	int round = 0;
	for( ; round < ROUND_COUNT ; ++round ) {
		fprintf(f , "  round %d\n" , round);
		auto const vector_available_ability_names
			=  you.generate_vector_of_available_ability_names();
		int selection = select_fprint_vector_of_strings(
				 stdout
				,vector_available_ability_names ); //TODO
		if( selection < 0 || selection >= (int)vector_available_ability_names.size() ) { // I'm (probably unneccesarily) afraid of casting here
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
		fprintf( f, "foe(%d):" , 0 );
		const auto foe_ability = foe.ptr_available_ability(0); /* ability[0] so only basic attack */
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
		you.perform_post_round_calculations();
		foe.perform_post_round_calculations();
		fprintf( f , "you: " );
		you.fprint(f);
		fprintf( f , "\n" );
		fprintf( f , "foe: " );
		foe.fprint(f);
		fprintf( f , "\n" );
		if( !(you.is_alive) ) {
			fprintf( f , "you died\n" );
			break;
		}
		if( !(foe.is_alive) ) {
			fprintf( f , "foe died\n" );
			break;
		}
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



void fprint_vector_of_combat_entities(
		 FILE * f
		,std::vector< CombatEntity > const &ce
		)
{
	size_t n = 0;
	for( auto const & pw : ce ) {
		fprintf( f , "0x%02zx " , n );
		pw.fprint(f);
		fprintf( f , "\n" );
		++n;
	}
}


void
fprint_vector_of_combat_entities(
		 FILE * f
		,std::vector< CombatEntity > &vec_ce
		)
{
	fprintf( f , "\n" );
	size_t n = 0;
	for( auto const &ce : vec_ce ) {
		fprintf( f , "%zu %zx"
				, n
				, n );
		ce.fprint(f);
		fprintf( f , "\n" );
		++n;
	}
	if( n != vec_ce.size() ) { /* some paranoid coding here, Mr. Linux Torvalds wouldn't be happy at that kind of defensive coding */
		fprintf( stderr ,  "BUG? in %s(): n != vec_ce.size()\n" , __func__ );
	} 
}





void fprint_vector_of_abilities(
		 FILE * f
		,std::vector< Ability > const &ab
		)
{
	size_t n = 0;
	for( auto const & a : ab ) {
		fprintf( f , "0x%02zx " , n );
		a.fprint(f);
		fprintf( f , "\n" );
		++n;
	}
}



int
select_fprint_vector_of_abilities(
		 FILE * f
		,std::vector< Ability > &vec_ab
		)
{
	fprint_vector_of_abilities(
			 f
			,vec_ab
			);
	fprintf( f , "input selection:\n" ); // TODO: better system for handling selections
	int selection = -1;
	scanf( "%i" , &selection );
	return selection;
}






void fight_versus_vectors(
		 FILE * f
		,std::vector< CombatEntity > &player_warriors
		,std::vector< CombatEntity > &enemy_warriors
		)
{
	if( player_warriors.size() <= 0 ) {
		fprintf( f , "cannot start fight with 0 player_warriors\n" );
		return;
	}
	assert( player_warriors.size() == 1 ); /* only 1 player character supported for now */
	if( enemy_warriors.size() <= 0 ) {
		fprintf( f , "cannot start fight with 0 enemy_warriors\n" );
		return;
	}
	fprintf( f , "player warriors %zu:\n" , player_warriors.size());
	fprint_vector_of_combat_entities(f , player_warriors);
	fprintf( f , "enemy warriors %zu:\n" , enemy_warriors.size() );
	fprint_vector_of_combat_entities(f , enemy_warriors);

	int round = 0;
	for( ; round < ROUND_COUNT ; ++round ) {
		auto player_warrior = player_warriors.at(0);
		player_warrior.fprint_available_abilities(f);
		SelectionResult sel0 = SelectionResult();
		sel0.fprint(f);
		fprintf( f, "\n" );
		int const selection_ability = sel0.get_integer();
		if( selection_ability < 0 ) {
			fprintf( f, "exiting, since you selected %d\n" , selection_ability );
			goto jump_end;
		}
		if( player_warrior.arr_is_ability_available[selection_ability] ) {
			fprintf( f , "\n" );
			TABLE_ABILITIES[selection_ability].fprint(f);
			fprintf( f , "\n" );
		} else {
			fprintf( f, "unavailable ability with id %d\n" , selection_ability );
		}
		/* int const target_selection */
		/* 	= select_fprint_vector_of_combat_entities( */
		/* 		FILE * f */
		/* 		,std::vector< CombatEntity > &vec_ce */
		/* 		); */
	}


jump_end:
	return;
}






void
perform_example_combat(FILE * f)
{
	CHECK_TABLE_ABILITY();
	CHECK_TABLE_ITEM_BASE();
	printf( "\n\n" );
	fprint_table_item_base_name_only(f);


	PlayerEntity player;
	//print_table_ability(f);
	struct CombatEntity &you = player.vector_warriors.at(0);;
	ItemEntity item_0 = ItemEntity(
			  4
			,+1
			, 0
			);
	ItemEntity item_1 = ItemEntity(
			  6
			,+2
			, 0
			);
	you.equip( f , item_0 );
	you.equip( f , item_1 );
	you.fprint_equipped(f);
	you.calculate_equipment_bonuses();
	fprintf( f , "\n" );
	you.fprint_rollmods_equipped_only_nonzero(f);
	you.stat[stat_type_strength] =  2;
	you.stat[stat_type_dexterity] = 4;
	you.stat[stat_type_wisdom] = 2;
	std::vector<CombatEntity>enemy_warriors = {
		 generate_enemy_of_level(2)
		,generate_enemy_of_level(2)
	};



	fight_versus_vectors(
			f
			,player.vector_warriors
			,enemy_warriors
			);


}


