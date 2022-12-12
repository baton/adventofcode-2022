#include <iostream>
#include <fstream>
#include <string>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	size_t cycle{ 0 };
	int value{ 1 }, sum{ 0 };

	while( !ifs.eof() )
	{
		if( ( ++cycle % 40 ) == 20 )
			sum += cycle * value;

		std::string s;
		ifs >> s;

		if( s == "addx" )
		{
			if( ( ++cycle % 40 ) == 20 )
				sum += cycle * value;

			int x{ 0 };
			ifs >> x;
			value += x;
		}
	}

	std::cout << "result = " << sum << std::endl;

	return 0;
}