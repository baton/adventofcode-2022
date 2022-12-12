#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <memory>


using pos_t = std::pair< int, int >;


inline int signum( int const x )
{
	if( x < 0 )
		return -1;
	if( 0 < x )
		return 1;
	return 0;
}

int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	pos_t hpos{ 0, 0 }, tpos{ 0, 0 };
	std::set< pos_t > visited;
	visited.insert( tpos );

	while( !ifs.eof() )
	{
		std::string s;
		int m{ 0 };
		ifs >> s >> m;

		int dx{ 0 }, dy{ 0 };
		switch( s[ 0 ] )
		{
			case 'U': dy = -1; break;
			case 'D': dy =  1; break;
			case 'L': dx = -1; break;
			case 'R': dx =  1; break;
			default: break;
		}

		for( ; m != 0; --m )
		{
			hpos.first += dx;
			hpos.second += dy;

			if( std::abs( hpos.first - tpos.first ) > 1 )
			{
				tpos.first += signum( hpos.first - tpos.first );
				tpos.second = hpos.second;
			}
			else if( std::abs( hpos.second - tpos.second ) > 1 )
			{
				tpos.first = hpos.first;
				tpos.second += signum( hpos.second - tpos.second );
			}

			visited.insert( tpos );
		}
	}

	std::cout << "result = " << visited.size() << std::endl;

	return 0;
}