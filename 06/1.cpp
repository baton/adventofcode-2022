#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::string s;
	ifs >> s;

	std::string::const_iterator i( s.cbegin() );
	for( ; i != s.cend() - 4; ++i )
	{
		std::string t( i, i + 4 );
		std::sort( t.begin(), t.end() );
		if( std::unique( t.begin(), t.end() ) == t.end() )
			break;
	}

	auto const result( std::distance( s.cbegin(), i ) + 4 );

	std::cout << "result = " << result << std::endl;;

	return 0;
}