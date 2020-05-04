#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "d16.h"



struct Character {
	int level_stat = 0;
	int level_skill = 0;
	int progres_skill = 0;
	int progres_next = 1;
	void print();
	void add_progres(int const);
};



void Character::print() {
	printf( "(t %d; k %d; p %d/%d)\n"
			,level_stat
			,level_skill
			,progres_skill
			,progres_next );
}


int
calc_progres_for_next_level(int const level) {
	return (2 << level);
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
	const char * name;
	int modifier_difficulty;
};


const Action
TABLE_ACTION[] = {
	{ "Trivial   " , 0 } ,
	{ "Easy      " , -2 } ,
	{ "Medium    " , -6 } ,
	{ "Difficult " , -12 } ,
	{ "Advanced  " , -20 } ,
};
size_t const SIZEOF_TABLE_ACTION = sizeof(TABLE_ACTION)/sizeof(TABLE_ACTION[0]);


void
print_action(const Action &a){
	printf( "%s  %3d\n" , a.name , a.modifier_difficulty );
}

void
print_actions_table(void) {
	for(size_t i = 0;
			i < SIZEOF_TABLE_ACTION;
			++i) {
		printf( "%3lu  " , i );
		print_action(TABLE_ACTION[i]);
	}
}


int main(int argc, char * argv[]) {
	srand(time(NULL));

	struct Character player_character;

	int opt;
	while ((opt = getopt(argc, argv, "t:k:p:")) != -1) {
		switch (opt) {
			case 't':
				player_character.level_stat = atoi(optarg);
				break;
			case 'k':
				player_character.level_skill = atoi(optarg);
				break;
			case 'p':
				player_character.progres_skill = atoi(optarg);
				break;
			default: /* '?' */
				fprintf(stderr, "Usage: %s [-t stat] [-k skill] [-p progres]\n",
						argv[0]);
				exit(EXIT_FAILURE);
		}
	}


	player_character.print();
	print_actions_table();
	int sel , ret = 0;
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
			printf( "Impossible(required_roll>0x%x). Aborting action.\n"
					, DICESIDES);
		} else {
			printf( "roll:" );
			const RollResult roll = RollResult( add , 1 );
			print_rollresult( roll );
			if( roll.success_level <= 0 ) {
				player_character.add_progres( 1 );
			}
		}

		ret = scanf( "%d" , &sel );
	}

	exit(EXIT_SUCCESS);
}



void
display_list_of_all_tries(
		int const success_multiply  = DEFAULT_SUCCESS_MULTIPLY
		,int const success_add = DEFAULT_SUCCESS_ADD )
{
	printf( "success_multiply=%2d; success_add=%d; required_for_success=%d\n"
			,success_multiply
			,success_add
			,d16_required_roll_for_success(success_add) );
	RollResult rr0 = RollResult(success_add , success_multiply);
	print_rollresult(rr0);
	RollResult rr1 = RollResult(roll_d16() , success_add , success_multiply);
	print_rollresult(rr1);

	RollResult arr_rollresult[DICESIDES];
	for( int i = 0; i < DICESIDES; ++i  ) {
		printf("%2d " , i);
		arr_rollresult[i] = RollResult( i , success_add , success_multiply );
		print_rollresult(arr_rollresult[i]);
	}
}
