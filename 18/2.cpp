#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <set>
#include <list>
#include <numeric>


struct cube_t
{
	int8_t x, y, z;

	inline bool operator<( cube_t const & r ) const
	{
		if( x < r.x )
			return true;
		if( r.x < x )
			return false;
		if( y < r.y )
			return true;
		if( r.y < y )
			return false;
		if( z < r.z )
			return true;
		if( r.z < z )
			return false;
		return false;
	}

	inline cube_t operator+( cube_t const & r ) const
	{
		return { x + r.x, y + r.y, z + r.z };
	}
};


cube_t const deltas[ 6 ] =
{
	{ -1,  0,  0 },
	{  1,  0,  0 },
	{  0, -1,  0 },
	{  0,  1,  0 },
	{  0,  0, -1 },
	{  0,  0,  1 }
};

inline bool is_edge( cube_t const & c )
{
	return ( c.x == 1 ) || ( c.y == 1 ) || ( c.z == 1 )
		|| ( c.x == 19 ) || ( c.y == 19 ) || ( c.z == 19 );
}

inline bool has_way_out( cube_t const & c, std::set< cube_t > const & cubes, std::set< cube_t > & visited, std::set< cube_t > & outer )
{
	std::list< cube_t > front;
	front.push_back( c );

	while( !front.empty() )
	{
		cube_t const x( front.front() );
		front.pop_front();

		visited.insert( x );

		for( cube_t const & d : deltas )
		{
			cube_t const p( x + d );
			if( cubes.count( p ) )
				continue;
			if( is_edge( p ) )
				return true;
			if( outer.count( p ) )
				return true;
			if( !visited.count( p ) )
				front.push_front( std::move( p ) );
		}
	}

	return false;
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::set< cube_t > cubes;

	std::ifstream ifs( argv[ 1 ] );
	while( !ifs.eof() )
	{
		std::string s;
		ifs >> s;

		int8_t x, y, z;
		std::sscanf( s.c_str(), "%hhu,%hhu,%hhu", &x, &y, &z );

		cubes.insert( { x, y, z } );
	}

	std::set< cube_t > inner, outer;

	uint64_t const result{
		std::transform_reduce(
			cubes.begin(), cubes.end(),
			0ULL,
			std::plus{},
			[ &cubes, &inner, &outer ]( cube_t const & c ) -> uint64_t
			{
				uint64_t n{ 0 };
				for( cube_t const & d : deltas )
				{
					cube_t const p( c + d );
					if( !cubes.count( p ) && !inner.count( p ) )
					{
						if( !outer.count( p ) )
						{
							std::set< cube_t > visited;
							if( has_way_out( p, cubes, visited, outer ) )
							{
								++n;

								outer.insert( visited.begin(), visited.end() );
							}
							else
							{
								inner.insert( std::move( p ) );
								inner.insert( visited.begin(), visited.end() );
							}
						}
						else
							++n;
					}
				}
				return n;
			}
		)
	};

	std::cout << "result = " << result << std::endl;

	return 0;
}