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
	struct {
		int level_stat = 0 ;
		int level_skill = 0;
		int progres_skill = 0;
	} minigame;

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
				minigame.level_stat = atoi(optarg);
				break;
			case 'k':
				minigame.level_skill = atoi(optarg);
				break;
			case 'p':
				minigame.progres_skill = atoi(optarg);
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


	minigame_crafting(
			 minigame.level_stat
			,minigame.level_skill
			,minigame.progres_skill
			);
}



