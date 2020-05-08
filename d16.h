#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int const DICESIDES = 0x10; //don't change this - many calculations depend on it
int const DEFAULT_ADDER = 1; // the plan was to add 1, if the score succeeds, so that the lowest successful ratio is 2/16, instead of 1/16 // okay, it works now :]




int
roll_d16(void);


struct RollResult {
	int success_level;
	int success_score; // maybe this should be storable as negatives? success_level of 0 should be enough
	int diceroll;
	int add;
	int multiply;
	bool is_success() const { return (success_level > 0); }
	RollResult() {}
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




int
calc_d16_score(
		 int const add
		,int const multiply
		);

int
calc_d16_success_level_from_score(
		 int const score );

int
roll_d16(void);

void
print_rollresult(
		const RollResult &rr);



int
d16_required_roll_for_success(
		int const );


