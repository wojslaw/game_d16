#include "d16.h"


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





// RollResult::RollResult() { }

RollResult::RollResult(
		 int const _diceroll
		,int const _success_add
		,int const _success_multiply )
{
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
