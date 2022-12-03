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

	std::vector< std::string > lines{ std::istream_iterator< line_t >( ifs ), std::istream_iterator< line_t >() };

	uint64_t result{ 0 };
	for( std::vector< std::string >::iterator i( lines.begin() ); i != lines.end(); )
	{
		std::string & a( *i );
		++i;
		std::string & b( *i );
		++i;
		std::string & c( *i );
		++i;

		std::sort( a.begin(), a.end() );
		std::sort( b.begin(), b.end() );
		std::sort( c.begin(), c.end() );

		std::string ab;
		std::set_intersection(
			a.begin(), a.end(),
			b.begin(), b.end(),
			std::back_inserter( ab )
		);

		std::string abc;
		std::set_intersection(
			ab.begin(), ab.end(),
			c.begin(), c.end(),
			std::back_inserter( abc )
		);

		auto w = []( char const c ) -> uint64_t
		{
			if( c <= 'Z' )
				return c - 'A' + 27;
			else
				return c - 'a' + 1;
		};

		result += w( abc[ 0 ] );
	}

	std::cout << "result = " << result << std::endl;

	return 0;
}