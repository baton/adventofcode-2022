#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>


struct line_t : std::string
{};

std::istream & operator>>( std::istream & is, line_t & line )
{
	return std::getline( is, line );
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	auto const result{
		std::count_if(
			std::istream_iterator< line_t >( ifs ), std::istream_iterator< line_t >(),
			[]( std::string const & s ) -> bool
			{
				unsigned int a{ 0 }, b{ 0 }, c{ 0 }, d{ 0 };
				std::sscanf( s.c_str(), "%d-%d,%d-%d", &a, &b, &c, &d );

				return ( ( a >= c ) && ( a <= d ) )
					|| ( ( c >= a ) && ( c <= b ) );
			}
		)
	};

	std::cout << "result = " << result << std::endl;

	return 0;
}