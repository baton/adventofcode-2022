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

	std::array< pos_t, 10 > rope{ { { 0, 0 } } };
	std::set< pos_t > visited;
	visited.insert( rope.back() );

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
			rope.front().first += dx;
			rope.front().second += dy;

			for( size_t i( 0 ); i != rope.size() - 1; ++i )
			{
				pos_t & hpos( rope[ i ] );
				pos_t & tpos( rope[ i + 1 ] );

				if( ( std::abs( hpos.first - tpos.first ) > 1 )
					|| ( std::abs( hpos.second - tpos.second ) > 1 ) )
				{
					tpos.first += signum( hpos.first - tpos.first );
					tpos.second += signum( hpos.second - tpos.second );
				}
			}

			visited.insert( rope.back() );
		}
	}

	std::cout << "result = " << visited.size() << std::endl;

	return 0;
}