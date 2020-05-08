#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "d16.h"
#include "minigame.h"









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



