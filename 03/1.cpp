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

	uint64_t const result{
		std::transform_reduce(
			std::istream_iterator< line_t >( ifs ), std::istream_iterator< line_t >(),
			0ULL,
			std::plus{},
			[]( std::string const & s ) -> char
			{
				std::string a( s.begin(), s.begin() + s.length() / 2 );
				std::string b( s.begin() + s.length() / 2, s.end() );
				std::sort( a.begin(), a.end() );
				std::sort( b.begin(), b.end() );

				std::string d;
				std::set_intersection(
					a.begin(), a.end(),
					b.begin(), b.end(),
					std::back_inserter( d )
				);

				char const & c( d[ 0 ] );
				if( c <= 'Z' )
					return c - 'A' + 27;
				else
					return c - 'a' + 1;
			}
		)
	};

	std::cout << "result = " << result << std::endl;

	return 0;
}