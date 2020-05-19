#pragma once
#include <stdio.h>
#include <vector>
#include <string>
#include <assert.h>

void
fprint_vector_of_strings(
		 FILE * f
		,std::vector< const char * > &vector_strings
		);

int
select_fprint_vector_of_strings(
		 FILE * f
		,std::vector< const char * > vector_strings
		);
