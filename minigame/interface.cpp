#include "interface.hpp"


SelectionResult::SelectionResult() {
	std::cin >> string;
	if(string.size() == 0) {
		return;
	}
	type = selection_result_type_string;
	char * endptr;
	int const in = strtol( string.c_str() , (&endptr) , 0 );
	if( (endptr - string.c_str()) > 0 ) { /* TODO check for errors too, not only whether conversion happened */
		type = selection_result_type_integer;
		integer = in;
	}
}


void
SelectionResult::fprint(FILE * f) const {
	switch(type) {
		case selection_result_type_none:
			fprintf( f , "selection_result_type_none" );
			break;
		case selection_result_type_integer:
			fprintf( f , "integer: 0x%x %d" , integer , integer );
			break;
		case selection_result_type_string:
			fprintf( f , "string: %s" , string.c_str() );
			break;
		default:
			fprintf( f , "unexpected selection_result_type: %d" , type );
	}
}



void
fprint_vector_of_strings(
		 FILE * f
		,std::vector< const char * > &vector_strings
		)
{
	fprintf( f ,  "\n" );
	int i = 0;
	for( auto s : vector_strings ) {
		fprintf( f , "%d %s\n"
				, i
				, s );
		++i;
	}
	assert((size_t)i == vector_strings.size() );
	/* if( (size_t)i != vector_strings.size() ) { */
	/* 	fprintf( stderr , "BUG? in fprint_vector_of_strings(): (size_t)i != vector_strings.size()\n" ); */
	/* } */
	assert( i == (int)vector_strings.size() );
	/* if( i != (int)vector_strings.size() ) { */
	/* 	fprintf( stderr , "BUG? in fprint_vector_of_strings(): i != (int)vector_strings.size()\n" ); */
	/* } */
}


int
get_selection_number(void)
{
	int selection = -1;
	scanf( "%i" , &selection );
	return selection;
}



int
select_fprint_vector_of_strings(
		 FILE * f
		,std::vector< const char * > vector_strings
		)
{
	fprint_vector_of_strings(
			 f
			,vector_strings
			);
	fprintf( f , "input selection:\n" ); // TODO: better system for handling selections
	return get_selection_number();
}
