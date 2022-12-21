#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <memory>
#include <array>
#include <algorithm>
#include <iterator>


using brick_t = std::pair< int, int >;

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

constexpr size_t const total_rocks{ 2022 };


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


bricks_t spawn( int top )
{
	static size_t i{ 0 };

	bricks_t new_rock;

	for( auto const & [ x, y ] : rocks[ i++ % _countof( rocks ) ] )
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


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::string jets;
	ifs >> jets;

	bricks_t bricks;

	for( size_t step{ 0 }, m{ 0 }; step != total_rocks; ++step )
	{
		bricks_t rock( spawn( bricks.empty() ? 0 : bricks.begin()->second ) );

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
	}

	std::cout << "result = " << ( bottom_floor - bricks.begin()->second ) << std::endl;

	return 0;
}