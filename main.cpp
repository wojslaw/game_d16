#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int const DICESIDES = 16;
int const DEFAULT_ADDER = 1;
int const DEFAULT_SUCCESS_ADD = -4;
int const DEFAULT_SUCCESS_MULTIPLY = 4;

typedef struct ratio Ratio;

struct ratio {
	union {
		int m;
		int multiplier;
	};
	union {
		int d;
		int divisor;
	};
};



Ratio ratio_d16_calc(
		 int const success_multiply
		,int const modifier) {
	// multiplier = (success_multiply * (ADDER + success_add))
	// divisor =  dice_sides
	//printf("[[%d;%d]]", success_multiply, modifier);
	Ratio r = {
		.m = (DEFAULT_ADDER + modifier) * success_multiply ,
		.d = DICESIDES
	};
	if( modifier  < 0) {
		r.m = 0;
	}
	return r;
}

void
ratio_print(Ratio const r) {
	printf("%d/%d" , r.m , r.d);
}


int d16_success_level(
		 int const base_level
		,int const modifier) {
	Ratio r = ratio_d16_calc(base_level, modifier);
	int const integer   = (r.m / r.d);
	int const remainder = (r.m % r.d);
	return
		(remainder > 0)
		? integer + 1
		: integer;
}

int d16_success_level_from_ratio(
		 Ratio const &r )
{
	int const integer   = (r.m / r.d);
	int const remainder = (r.m % r.d);
	return
		(remainder > 0)
		? integer + 1
		: integer;
}



struct RollResult {
	Ratio calc_ratio;
	int diceroll;
	int success_level;
	RollResult();
	RollResult(
			 int const _diceroll
			,int const _success_add
			,int const _success_multiply );
};

RollResult::RollResult() { }

RollResult::RollResult(
		 int const _diceroll
		,int const _success_add
		,int const _success_multiply )
{
	//printf( "(((%d,%d,%d)))" ,_diceroll, _success_add ,_success_multiply );
	diceroll = _diceroll;
	calc_ratio = ratio_d16_calc(_success_multiply , _success_add + diceroll);
	success_level = d16_success_level_from_ratio(calc_ratio);
}

void
print_rollresult(
		const RollResult &rr)
{
	printf( "%d(d16:%#x;score:%#x)\n"
			,rr.success_level
			,rr.diceroll
			,rr.calc_ratio.multiplier);
}




int d16_required_roll_for_success(
		int const modifier ) {
	return (-modifier);
}


int main(int argc, char * argv[]) {

	int success_multiply  = DEFAULT_SUCCESS_MULTIPLY;
	int success_multiply_set = 0;
	int success_add = DEFAULT_SUCCESS_ADD;
	int success_add_set = 0;


	int opt;
	while ((opt = getopt(argc, argv, "a:d:")) != -1) {
		switch (opt) {
			case 'a':
				success_add = atoi(optarg);
				success_add_set = 1;
				break;
			case 'd':
				success_multiply = atoi(optarg);
				success_multiply_set = 1;
				break;
			default: /* '?' */
				fprintf(stderr, "Usage: %s [-a success_add] [-d success_multiply]\n",
						argv[0]);
				exit(EXIT_FAILURE);
		}
	}


	printf( "success_multiply=%2d(%d); success_add=%d(%d); required_for_success=%d\n"
			,success_multiply,  success_multiply_set
			,success_add, success_add_set
			,d16_required_roll_for_success(success_add) );

	Ratio arr_ratios[DICESIDES];
	int arr_success_level[DICESIDES];
	RollResult arr_rollresult[DICESIDES];
	for( int i = 0; i < DICESIDES; ++i  ) {
		printf("%2d " , i);
		//arr_success_level[i] = d16_success_level( success_multiply , success_add + i );
		//printf("%d " , arr_success_level[i]);
		//arr_ratios[i] = ratio_d16_calc( success_multiply , success_add + i );
		//ratio_print(arr_ratios[i]);
		arr_rollresult[i] = RollResult( i , success_add , success_multiply );
		print_rollresult(arr_rollresult[i]);
	}
}
