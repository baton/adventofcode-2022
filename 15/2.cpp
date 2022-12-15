#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <memory>


using segment_t = std::pair< int, int >;


int main( int argc, char * argv[] )
{
	if( argc < 3 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );
	int const max_xy( std::atoi( argv [ 2 ] ) );

	std::vector< std::set< segment_t > > observed( 4000001 );

	int min_x{ max_xy }, max_x{ 0 }, min_y{ max_xy }, max_y{ 0 };

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
		if( min_y > ( sy - md ) )
			min_y = sy - md;
		if( max_y < ( sy + md ) )
			max_y = sy + md;

		for( int y( std::max( 0, sy - md ) ); y != std::min( 4000000, sy + md ); ++y )
		{
			int x1( sx - ( md - std::abs( y - sy ) ) );
			int x2( sx + ( md - std::abs( y - sy ) ) );
			observed[ y ].emplace( x1, x2 );
		}
	}

	min_x = std::max( 0, min_x );
	max_x = std::min( 4000000, max_x );
	min_y = std::max( 0, min_y );
	max_y = std::min( 4000000, max_y );

	for( int y( 0 ); y != 4000001; ++y )
	{
		std::set< segment_t > & o( observed[ y ] );

		std::vector< segment_t > oo;
		std::set< segment_t >::iterator i( o.begin() );
		while( i != o.end() )
		{
			segment_t s( *i );
			std::set< segment_t >::iterator j( std::next( i ) );
			while( ( j != o.end() ) && ( j->first <= s.second ) )
			{
				s.second = std::max( s.second, j->second );
				j = o.erase( j );
			}
			if( !oo.empty() )
			{
				if( oo.back().second == s.first - 2 )
				{
					std::cout << "result = " <<  static_cast< uint64_t >( s.first - 1 ) * 4000000 + static_cast< uint64_t >( y ) << std::endl;
				}
			}
			oo.push_back( s );
			++i;
		}
	}

	return 0;
}