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


int
RollResult::get_score_roll(void) const {
	return roll_score;
}


int
RollResult::get_score_success(void) const {
	return success_score;
}




struct TestSet {
	int diceroll;
	int add;
	int multiply;
	int expected_success_level;
	TestSet(
			 int const _diceroll
			,int const _add
			,int const _multiply
			,int const _expected_success_level) {
		diceroll = _diceroll;
		add = _add;
		multiply = _multiply;
		expected_success_level = _expected_success_level;
	}
	int fprint(FILE * f) const {
		return fprintf( f , "[d 0x%x , a %d, m %d, expect %d]"
				,diceroll
				,add
				,multiply
				,expected_success_level );
	}
};



void devd16_perform_tests(void) {
	fprintf(stderr , "devd16_perform_tests\n");
	const TestSet set[] = {
		TestSet( 0x0 , 0 , 0 , 0) ,
		TestSet( 0x1 , 0 , 0 , 1) ,
		TestSet( 0xf , 0 , 0 , 1) ,
		TestSet( 0x0 , 1 , 1 , 1) ,
		TestSet( 0xf , 1 , 1 , 2) ,
		TestSet( 0x1 , 1 , 2 , 1) ,
		TestSet( 0x7 , 8 , 2 , 2) ,
		TestSet( 0x0 , 0 , -1 , 0) ,
		TestSet( 0x0 , 1 , -1 , 0) ,
	};
	size_t test_id = 0;
	int failures = 0;
	for( auto & s : set ) {
		auto const rr = RollResult(
				 s.diceroll
				,s.add
				,s.multiply
				);
		int const success_level = rr.get_success_level();
		if( rr.get_success_level() != s.expected_success_level ) {
			fprintf( stderr, "failed test 0x%zx " , test_id);
			s.fprint( stderr );
			fprintf( stderr
					," got %d (scores: roll=%d , success=%d)\n"
					,success_level
					,rr.roll_score
					,rr.success_score
					);

			++failures;
		}
		++test_id;
	}

	fprintf( stderr, "done.");
	if( failures > 0 ) {
		fprintf( stderr, " failed tests: %d\n" , failures );
	} else {
		fprintf( stderr, " all ok");
	}
}


void
fprint_rollresult_prediction(
		FILE * f
		,int const add
		,int const multiply
		)
{
	/* TODO test */
	fprintf( f , "for (a %d , m %d)"
			,add
			,multiply );
	int const required_roll = d16_required_roll_for_success( add );
	if( required_roll > ROLLMAX ) {
		fprintf( f , "impossible. requires roll 0x%x" , required_roll );
		return;
	} else if( required_roll <= ROLLMIN) {
		fprintf( f , "success guaranteed. requires roll 0x%x" , required_roll );
	} else {
		fprintf( f , "requires roll 0x%x" , required_roll );
	}

	fprintf( f , "range of success levels:" );
	auto const roll_min = RollResult( required_roll , add , multiply );
	auto const roll_max = RollResult( ROLLMAX , add , multiply );
	auto const succ_min = roll_min.get_success_level();
	auto const succ_max = roll_max.get_success_level();
	fprintf( f , "[ %d ; %d ]"
			, succ_min
			, succ_max
		   );
}
