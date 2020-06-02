#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "d16.h"
#include "minigame/crafting/crafting.h"
#include "minigame/combat/combat.h"








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
	enum minigame_type {
		minigame_type_crafting_economy ,
		minigame_type_combat ,
	};
	enum minigame_type minigame_type = minigame_type_combat;

	int opt;
	while ((opt = getopt(argc, argv, "fDBTt:k:p:M:A:")) != -1) {
		switch (opt) {
			case 'T':
				display_table_item_type(stderr);
				break;
			case 'B':
				display_table_item_base(stderr);
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
			case 'f':
				devd16_perform_tests();
				exit(EXIT_SUCCESS);
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


	switch(minigame_type) {
		case minigame_type_crafting_economy:
			minigame_crafting(
					minigame.level_stat
					,minigame.level_skill
					,minigame.progres_skill
					);
			break;
		case minigame_type_combat:
			minigame_combat(stdout);
			break;
		default:
			fprintf(stderr , "unrecognized minigame type:%d\n" , minigame_type);
	}
}



