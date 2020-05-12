#include "crafting.h"




const Action
TABLE_ACTION[] = {
	{  0  , 1 ,  "Trivial"   ,  } ,
	{ -2  , 2 ,  "Easy"      ,  } ,
	{ -6  , 4 ,  "Medium"    ,  } ,
	{ -12 , 6 ,  "Difficult" ,  } ,
	{ -16 , 8 ,  "Advanced"  ,  } ,
	{ -20 ,12 ,  "Adept"  ,  } ,
	{ -24 ,16 ,  "Expert"  ,  } ,
};
size_t const SIZEOF_TABLE_ACTION = sizeof(TABLE_ACTION)/sizeof(TABLE_ACTION[0]);










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



void
minigame_crafting(
		 int const level_stat
		,int const level_skill
		,int const progres_skill
		)
{
	struct Character player_character;
	player_character.level_stat    = level_stat;
	player_character.level_skill   = level_skill;
	player_character.progres_skill = progres_skill;

	printf( "welcome to d16 minigame!\n" );
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
