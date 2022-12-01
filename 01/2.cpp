#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	uint64_t c{ 0 };
	std::vector< uint64_t > v;
	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );
		if( s.empty() || ifs.eof() )
		{
			v.push_back( c );
			c = 0;
		}
		else
			c += std::stoull( s );
	}

	std::sort( v.begin(), v.end() );
	uint64_t result{ 0 };
	result += v.back();
	v.pop_back();
	result += v.back();
	v.pop_back();
	result += v.back();
	v.pop_back();

	std::cout << "result = " << result << std::endl;

	return 0;
}