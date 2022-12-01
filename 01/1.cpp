#include <iostream>
#include <fstream>
#include <string>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	uint64_t m{ 0 }, c{ 0 };
	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );
		if( s.empty() || ifs.eof() )
		{
			if( m < c )
				m = c;
			c = 0;
		}
		else
			c += std::stoull( s );
	}

	std::cout << "result = " << m << std::endl;

	return 0;
}