#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int const DICESIDES = 0x10; //don't change this - many calculations depend on it
int const DEFAULT_ADDER = 1; // the plan was to add 1, if the score succeeds, so that the lowest successful ratio is 2/16, instead of 1/16 // okay, it works now :]
int const DEFAULT_SUCCESS_ADD = -4;
int const DEFAULT_SUCCESS_MULTIPLY = 4;


int
calc_d16_score(
		 int const add
		,int const multiply
		)
{
	return
		add > 0
		? ((DEFAULT_ADDER + add) * multiply)
		: add;
}


int calc_d16_success_level_from_score(
		 int const score )
{
	if(score <= 0) {
		return 0;
	}
	int const integer   = (score / DICESIDES);
	int const remainder = (score % DICESIDES);
	return
		(remainder > 0)
		? integer + 1
		: integer;
}

int
roll_d16(void)
{
	return (rand() % 16);
}




struct RollResult {
	int success_level;
	int success_score; // maybe this should be storable as negatives? success_level of 0 should be enough
	int diceroll;
	int add;
	int multiply;
	RollResult();
	RollResult(
			 int const _diceroll
			,int const _success_add
			,int const _success_multiply );
	RollResult(
			int const _success_add
			,int const _success_multiply )
		: RollResult(
				roll_d16()
				,_success_add
				,_success_multiply ) {}
};

RollResult::RollResult() { }

RollResult::RollResult(
		 int const _diceroll
		,int const _success_add
		,int const _success_multiply )
{
	printf("[[[[%d %d %d]]]]" , _diceroll, _success_add , _success_multiply);
	add = _success_add;
	multiply = _success_multiply;
	diceroll = _diceroll;
	success_score = calc_d16_score(_success_add + diceroll , _success_multiply );
	success_level = calc_d16_success_level_from_score(success_score);
}




void
print_rollresult(
		const RollResult &rr)
{
	printf( "%d (s%2d;d 0x%x;a %d;m %d)\n"
			,rr.success_level
			,rr.success_score
			,rr.diceroll
			,rr.add
			,rr.multiply
		  );
}




int d16_required_roll_for_success(
		int const modifier ) {
	return (-modifier);
}


int main(int argc, char * argv[]) {
	srand(0);
	int success_multiply  = DEFAULT_SUCCESS_MULTIPLY;
	int success_multiply_set = 0;
	int success_add = DEFAULT_SUCCESS_ADD;
	int success_add_set = 0;


	int opt;
	while ((opt = getopt(argc, argv, "a:m:")) != -1) {
		switch (opt) {
			case 'a':
				success_add = atoi(optarg);
				success_add_set = 1;
				break;
			case 'm':
				success_multiply = atoi(optarg);
				success_multiply_set = 1;
				break;
			default: /* '?' */
				fprintf(stderr, "Usage: %s [-a success_add] [-m success_multiply]\n",
						argv[0]);
				exit(EXIT_FAILURE);
		}
	}


	printf( "success_multiply=%2d(%d); success_add=%d(%d); required_for_success=%d\n"
			,success_multiply,  success_multiply_set
			,success_add, success_add_set
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
