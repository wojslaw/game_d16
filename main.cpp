#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "d16.h"






int main(int argc, char * argv[]) {
	srand(time(NULL));
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
