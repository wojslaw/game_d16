#pragma once
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include <iostream>


enum selection_result_type {
	selection_result_type_none ,
	selection_result_type_integer ,
	selection_result_type_string ,
};


struct SelectionResult {
	enum selection_result_type type = selection_result_type_none;
	std::string string;
	int integer = INT_MIN; /* hopefully, this will cause a fast crash if integer is read when it's not an integer */
	SelectionResult( );
	void fprint(FILE * f) const;
	int get_integer(void) const {
		assert( type == selection_result_type_integer );
		return integer;
	}
	const char * c_str(void) const {
		return string.c_str();
	}
};







int
get_selection_number(void);

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


