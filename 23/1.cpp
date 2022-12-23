#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <algorithm>
#include <limits>


constexpr size_t const steps{ 10 };


using pos_t = std::pair< int64_t, int64_t >;
using elves_t = std::vector< pos_t >;
using elves_map_t = std::map< pos_t, uint64_t >;
using propose_t = std::map< pos_t, std::list< uint64_t > >;

constexpr pos_t const all_directions[ 8 ]
{
	{ -1, -1 }, {  0, -1 }, {  1, -1 }, { -1, 0 }, { 1, 0 }, { -1,  1 }, {  0,  1 }, {  1,  1 }
};

constexpr pos_t const directions[ 4 ][ 4 ]
{
	{ { -1, -1 }, {  0, -1 }, {  1, -1 } }, // NW, N, NE -> N
	{ { -1,  1 }, {  0,  1 }, {  1,  1 } }, // SW, S, SE -> S
	{ { -1, -1 }, { -1,  0 }, { -1,  1 } }, // NW, W, SW -> W
	{ {  1, -1 }, {  1,  0 }, {  1,  1 } }, // NE, E, SE -> E
};

constexpr inline pos_t operator+( pos_t const & a, pos_t const & b )
{
	return { a.first + b.first, a.second + b.second };
}

inline bool is_alone( pos_t const & e, elves_map_t const ees )
{
	for( pos_t const & d : all_directions )
	{
		pos_t const ee{ e + d };
		if( ees.find( ee ) != ees.end() )
			return false;
	}
	return true;
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	int64_t y{ 0 };
	elves_t elves;
	uint64_t const elves_count{ 0 };

	std::ifstream ifs( argv[ 1 ] );
	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );

		for( int64_t x{ 0 }; x != s.length(); ++x )
			if( s[ x ] == '#' )
				elves.emplace_back( x, y );
		++y;
	}

	for( size_t step{ 0 }; step != steps; ++step )
	{
		propose_t proposes;

		elves_map_t eps;
		for( uint64_t ee{ 0 }; ee != elves.size(); ++ee )
			eps.emplace( elves[ ee ], ee );

		for( uint64_t e{ 0 }; e != elves.size(); ++e )
		{
			if( is_alone( elves[ e ], eps ) )
				continue;

			for( size_t dd{ 0 }; dd != 4; ++dd )
			{
				size_t const d{ ( step + dd ) % _countof( directions ) };
				bool found{ false };
				for( size_t i( 0 ); i != ( _countof( directions[ d ] ) - 1 ) && !found; ++i )
				{
					pos_t const ee{ elves[ e ] + directions[ d ][ i ] };
					found = eps.find( ee ) != eps.end();
				}
				if( !found )
				{
					proposes[ elves[ e ] + directions[ d ][ 1 ] ].push_back( e );

					break;
				}
			}
		}

		for( auto const & [ p, l ] : proposes )
			if( l.size() == 1 )
				elves[ l.front() ] = p;
	}

	pos_t a{ std::numeric_limits< int64_t >::max(), std::numeric_limits< int64_t >::max() };
	pos_t b{ std::numeric_limits< int64_t >::min(), std::numeric_limits< int64_t >::min() };
	for( pos_t const & e : elves )
	{
		if( a.first > e.first )
			a.first = e.first;
		if( a.second > e.second )
			a.second = e.second;
		if( b.first < e.first )
			b.first = e.first;
		if( b.second < e.second )
			b.second = e.second;
	}

	int64_t const result{ ( std::abs( b.first - a.first ) + 1 ) * ( std::abs( b.second - a.second ) + 1 ) - static_cast< int64_t >( elves.size() ) };

	std::cout << "result = " << result << std::endl;

	return 0;
}