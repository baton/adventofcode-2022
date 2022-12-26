#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <array>
#include <algorithm>
#include <iterator>


using brick_t = std::pair< int64_t, int64_t >;

int const rocks[ 5 ][ 5 ][ 2 ]{
	{
		{ 0, 0  }, // dummy

		{ 0, 0  }, { 1,  0 }, { 2,  0 }, { 3,  0 }
	}
	,
	{
		           { 1, -2 },
		{ 0, -1 }, { 1, -1 }, { 2, -1 },
		           { 1,  0 }
	}
	,
	{
		                      { 2, -2 },
		                      { 2, -1 },
		{ 0,  0 }, { 1,  0 }, { 2,  0 }
	}
	,
	{
		{ 0, 0  }, // dummy

		{ 0, -3 },
		{ 0, -2 },
		{ 0, -1 },
		{ 0,  0 }
	}
	,
	{
		{ 0, 0  }, // dummy

		{ 0, -1 }, { 1, -1 },
		{ 0,  0 }, { 1,  0 }
	}
};

constexpr int const bottom_floor{ 0 };
constexpr int const width{ 7 };
constexpr int const right_wall{ width + 1 };

constexpr int const rock_left{ 2 };
constexpr int const rock_bottom{ -3 };

constexpr int64_t const total_rocks{ 1000000000000ULL };


struct brick_cmp
{
	inline bool operator()( brick_t const & a, brick_t const & b ) const
	{
		if( a.second < b.second )
			return true;
		if( b.second < a.second )
			return false;
		if( a.first < b.first )
			return true;
		if( b.first < a.first )
			return false;
		return false;
	}
};

using bricks_t = std::set< brick_t, brick_cmp >;


bricks_t spawn( int64_t top, size_t const i )
{
	bricks_t new_rock;

	for( auto const & [ x, y ] : rocks[ i ] )
		new_rock.emplace( x + rock_left, y + top + rock_bottom - 1 );

	return new_rock;
}

inline bool can_move( bricks_t const & rock, bricks_t const & bricks, int const dx, int const dy )
{
	for( auto const & b : rock )
	{
		if( b.first + dx < 0 )
			return false;
		if( b.first + dx >= width )
			return false;
		if( b.second + dy >= 0 )
			return false;
		if( bricks.count( { b.first + dx, b.second + dy } ) )
			return false;
	}

	return true;
}

inline void move_rock( bricks_t & rock, int const dx, int const dy )
{
	bricks_t moved;
	for( auto const & b : rock )
		moved.emplace( b.first + dx, b.second + dy );
	rock = std::move( moved );
}


using db_t = std::map< std::tuple< std::array< uint8_t, 16 >, size_t, int64_t >, std::pair< int64_t, int64_t > >;


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::string jets;
	ifs >> jets;

	bricks_t bricks;

	db_t db;

	int64_t dtop{ 0 };

	int64_t stepx{ 0 }, dtopx{ 0 };

	for( int64_t step{ 0 }, m{ 0 }; step != total_rocks; ++step )
	{
		size_t const rock_i{ step % _countof( rocks ) };
		int64_t const m0{ static_cast< int64_t >( m % jets.size() ) };

		bricks_t rock( spawn( bricks.empty() ? 0 : bricks.begin()->second, rock_i ) );

		for( ; ; )
		{
			int const d{ jets[ m++ % jets.size() ] == '<' ? -1 : 1 };

			if( can_move( rock, bricks, d, 0 ) )
				move_rock( rock, d, 0 );

			if( can_move( rock, bricks, 0, 1 ) )
				move_rock( rock, 0, 1 );
			else
				break;
		}

		bricks.insert( rock.begin(), rock.end() );

		if( ( step > 0 ) && ( bricks.begin()->second < -16 ) )
		{
			db_t::key_type k{ { 0 }, rock_i, m0 };

			brick_t bb{ 0, bricks.begin()->second };
			for( size_t i{ 0 }; i != 16; ++i, ++bb.second )
			{
				uint8_t & r{ std::get< 0 >( k )[ i ] };
				for( bb.first = 0; bb.first != 7; ++bb.first )
					r |= bricks.count( bb ) << bb.first;
			}

			std::pair< db_t::iterator, bool > x{ db.try_emplace( k, step, bricks.begin()->second + dtop ) };
			if( !x.second )
			{
				int64_t const step0{ x.first->second.first };
				int64_t const skips{ ( ( total_rocks - step ) / ( step - step0 ) ) };

				step += skips * ( step - step0 );
				dtop -= skips * ( bricks.begin()->second - x.first->second.second );
			}
		}
	}

	std::cout << "result = " << ( bottom_floor - bricks.begin()->second + dtop ) << std::endl;

	return 0;
}