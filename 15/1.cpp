#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <memory>


using segment_t = std::pair< int, int >;


int main( int argc, char * argv[] )
{
	if( argc < 3 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );
	int const the_y( std::atoi( argv [ 2 ] ) );

	std::set< segment_t > observed;

	int min_x{ 10000000 }, max_x{ 0 };

	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );

		int sx, sy, bx, by;
		std::sscanf( s.c_str(), "Sensor at x=%i, y=%i: closest beacon is at x=%i, y=%i", &sx, &sy, &bx, &by );

		int const md( std::abs( bx - sx ) + std::abs( by - sy ) );

		if( min_x > ( sx - md ) )
			min_x = sx - md;
		if( max_x < ( sx + md ) )
			max_x = sx + md;

		if( ( sy - md <= the_y ) && ( the_y <= sy + md ) )
			observed.emplace( sx - ( md - std::abs( the_y - sy ) ), sx + ( md - std::abs( the_y - sy ) ) );
	}

	std::set< segment_t > oo;
	std::set< segment_t >::iterator i( observed.begin() );
	while( i != observed.end() )
	{
		segment_t s( *i );
		std::set< segment_t >::iterator j( std::next( i ) );
		while( ( j != observed.end() ) && ( j->first <= s.second ) )
		{
			s.second = std::max( s.second, j->second );
			j = observed.erase( j );
		}
		oo.insert( s );
		++i;
	}

	int onum{ 0 };
	for( auto const & [ a, b ] : oo )
		onum += b - a + 1;

	std::cout << "result = " << onum - 1 << std::endl;

	return 0;
}