#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <iterator>


struct line_t : std::string
{};

std::istream & operator>>( std::istream & is, line_t & line )
{
	return std::getline( is, line );
}

std::map< std::string, uint64_t > table
{
	{ "A X", 4 },
	{ "A Y", 8 },
	{ "A Z", 3 },
	{ "B X", 1 },
	{ "B Y", 5 },
	{ "B Z", 9 },
	{ "C X", 7 },
	{ "C Y", 2 },
	{ "C Z", 6 }
};


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	uint64_t const result{
		std::transform_reduce(
			std::istream_iterator< line_t >( ifs ), std::istream_iterator< line_t >(),
			0ULL,
			std::plus{},
			[]( std::string const & s ) -> uint64_t
			{
				return table[ s ];
			}
		)
	};

	std::cout << "result = " << result << std::endl;

	return 0;
}