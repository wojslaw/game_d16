#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "d16.h"

// Add 2 different multipliers:
//   1. score_multiplier( this is the multiplier that works currently )
//   2. success_level_multiplier( this will be multiplied times the success level, that is the value after rounding )
//   hmm, now that I think of it - maybe this would be too much unnecessary complexity?


struct Character {
	int level_stat = 0;
	int level_skill = 0;
	int progres_skill = 0;
	int progres_next = 2;
	int resources = 10;
	void print();
	void add_progres(int const);
};

int const DEFAULT_SUCCESS_ADD = -4;
int const DEFAULT_SUCCESS_MULTIPLY = 4;



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




struct Action {
	int modifier_difficulty;
	int cost;
	const char * name;
};


const Action
TABLE_ACTION[] = {
	{  0  , 1 ,  "Trivial"   ,  } ,
	{ -2  , 2 ,  "Easy"      ,  } ,
	{ -6  , 4 ,  "Medium"    ,  } ,
	{ -12 , 6 ,  "Difficult" ,  } ,
	{ -20 , 8 ,  "Advanced"  ,  } ,
};
size_t const SIZEOF_TABLE_ACTION = sizeof(TABLE_ACTION)/sizeof(TABLE_ACTION[0]);


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

void
display_table_weapon_type(FILE * f)
{
	for(int i = 0; i < SIZEOF_STRINGTABLE_WEAPON_TYPE; ++i) {
		fprintf(f , "%d %s\n" , i , STRINGTABLE_WEAPON_TYPE[i] );
	}
}

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
};


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


const
WeaponBase
TABLE_WEAPON_BASE[] = {
	{ .type=weapon_type_polearm , .range = 3, .base_damage = 6 , .name = "Spear" } ,
	{ .type=weapon_type_sword   , .range = 1, .base_damage = 3 , .name = "Shortsword" } ,
	{ .type=weapon_type_axe , .required_strength = 2 , .to_hit = -2 , .range = 1, .base_damage = 5 , .name = "Lumber Axe" } ,
};
size_t SIZEOF_TABLE_WEAPON_BASE (sizeof(TABLE_WEAPON_BASE)/sizeof(TABLE_WEAPON_BASE[0])) ;


void
display_table_weapon_base(FILE * f)
{
	for(size_t i = 0; i < SIZEOF_TABLE_WEAPON_BASE ; ++i ) {
		fprintf(f , "%lu" , i );
		TABLE_WEAPON_BASE[i].fprint(f);
		fprintf(f , "\n" );
	}
}






void
display_list_of_all_tries(
		FILE * f
		,int const score_add
		,int const score_multiply )
{
	fprintf( f ,"score_add=%d; required_for_score=%d\n"
			,score_add
			,d16_required_roll_for_success(score_add) );

	RollResult arr_rollresult[DICESIDES];
	for( int i = 0; i < DICESIDES; ++i  ) {
		fprintf( f , "%2d " , i);
		arr_rollresult[i] = RollResult( i , score_add , score_multiply );
		print_rollresult(arr_rollresult[i]);
	}
}


void
display_debugish_thing(void) {
	
}



int main(int argc, char * argv[]) {
	srand(time(NULL));

	struct Character player_character;
	bool flag_display_list_of_all_tries = false;
	int score_add = 0;
	int score_multiply = 0;

	int opt;
	while ((opt = getopt(argc, argv, "DBTt:k:p:M:A:")) != -1) {
		switch (opt) {
			case 'T':
				display_table_weapon_type(stderr);
				break;
			case 'B':
				display_table_weapon_base(stderr);
				break;
			case 't':
				player_character.level_stat = atoi(optarg);
				break;
			case 'k':
				player_character.level_skill = atoi(optarg);
				break;
			case 'p':
				player_character.progres_skill = atoi(optarg);
				break;
			case 'M':
				score_multiply = atoi(optarg);
				break;
			case 'A':
				score_add = atoi(optarg);
				break;
			case 'D':
				flag_display_list_of_all_tries = true;
				break;
			default: /* '?' */
				fprintf(stderr, "Usage: %s [-t stat] [-k skill] [-p progres]\n",
						argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	if(flag_display_list_of_all_tries) {
		display_list_of_all_tries(
				stdout
				,score_add
				,score_multiply);
		exit(EXIT_SUCCESS);
	}

	player_character.print();
	print_actions_table();
	int sel , ret = 0;
	int total_cost = 0;
	int total_income = 0;
	ret = scanf( "%d" , &sel );
	while(
			 (sel >= 0)
			&& (sel < (int)SIZEOF_TABLE_ACTION)
			&& (ret != EOF)
			&& (ret > 0)
			) {
		print_action(TABLE_ACTION[sel]);
		player_character.print();
		int const add =
			  (player_character.level_stat)
			+ (player_character.level_skill)
			+ (TABLE_ACTION[sel].modifier_difficulty);
		int const required_roll = d16_required_roll_for_success( add );
		printf( "add:%d; required_roll:%d; "
				, add
				, required_roll );
		if( required_roll >= DICESIDES ) {
			printf( "Impossible(required_roll>=0x%x). Aborting action.\n"
					, DICESIDES);
		} else {
			int const cost = TABLE_ACTION[sel].cost;
			total_cost += cost;
			player_character.resources -= cost;
			printf( "roll:" );
			const RollResult roll = RollResult( add , 1 );
			print_rollresult( roll );
			if(  (roll.is_success()) ) {
				int const income = ((2 + roll.success_level) * cost);
				player_character.resources += income;
				total_income += income;
			} else {
				player_character.add_progres( 1 );
				player_character.print();
			}
			if( player_character.resources < 0 ) {
				goto jump_end;
			}
		}

		ret = scanf( "%d" , &sel );
	}


	printf( "\nGame ended\ntotal_cost=%d ; total_income = %d\n" , total_cost , total_income );

jump_end:
	if(player_character.resources < 0) {
		printf( "You lost, because you went into negative money and failed to recover!\n" );
	}
}



