#include "d16.h"


int
calc_d16_score(
		 int const add
		,int const multiply
		)
{
	/* currently, a multiply of 0 will clearly ensure the success is always 0, but maybe it should default to 1? or, if multiply is less than 1, then it is calculated normally, but success cannot be raised higher than 1? */
	int score = add;
	if( score > 0 ) {
		if( multiply > 0 ) {
			score = ((DEFAULT_ADDER + add) * multiply);
		} else if( multiply == 0 ) {
			score = 1; /* this special interaction(if `multiply=0` then `score` cannot be higher than `1`) will need to be documented for user */
		} else {
			score = 0;
		}
	}
	return score;
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
	roll_score = diceroll + add;
	success_score = calc_d16_score(roll_score , _success_multiply );
	success_level = calc_d16_success_level_from_score(success_score);
}




void
print_rollresult(
		const RollResult &rr)
{
	printf( "%d (s%2d ; d 0x%x ; a %d ; m %d)"
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


void
RollResult::fprint(FILE * f) const {
// print_rollresult(*this); // hmm, weird quirk of C++ : you have to dereference here, because `this` is a pointer // what does this comment actually mean??? is it outdated??
	fprintf( f , "(a%3d, m%3d, d0x%X, rs%2d, ss%d, sl%d)"
			,add
			,multiply
			,diceroll
			,roll_score
			,success_score
			,success_level
		  );
}


int
RollResult::get_success_level(void) const {
	return success_level;
};
