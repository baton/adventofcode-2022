#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <set>
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

	uint64_t const result{
		std::transform_reduce(
			cubes.begin(), cubes.end(),
			0ULL,
			std::plus{},
			[ &cubes ]( cube_t const & c ) -> uint64_t
			{
				uint64_t n{ 0 };
				for( cube_t const & d : deltas )
					n += 1 - cubes.count( c + d );
				return n;
			}
		)
	};

	std::cout << "result = " << result << std::endl;

	return 0;
}