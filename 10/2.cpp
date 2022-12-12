#include <iostream>
#include <fstream>
#include <string>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	int cycle{ 0 };
	int value{ 1 }, sum{ 0 };

	std::string screen;

	while( !ifs.eof() )
	{
		++cycle;
		screen.push_back(
			( ( cycle - 1 ) % 40 >= ( value - 1 ) ) && ( ( cycle - 1 ) % 40 <= ( value + 1 ) ) ? '#' : '.' );

		std::string s;
		ifs >> s;

		if( s == "addx" )
		{
			++cycle;
			screen.push_back(
				( ( cycle - 1 ) % 40 >= ( value - 1 ) ) && ( ( cycle - 1 ) % 40 <= ( value + 1 ) ) ? '#' : '.' );

			int x{ 0 };
			ifs >> x;
			value += x;
		}
	}

	for( size_t i( 0 ); i != screen.length(); )
	{
		std::cout << screen[ i ];
		if( ++i % 40 == 0 )
			std::cout << std::endl;
	}

	return 0;
}