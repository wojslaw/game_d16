#include "interface.hpp"


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
	int selection = -1;
	scanf( "%i" , &selection );
	return selection;
}
