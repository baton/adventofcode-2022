#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <iterator>
#include <memory>
#include <algorithm>


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::vector< std::string > const field{ std::istream_iterator< std::string >( ifs ),
		std::istream_iterator< std::string >() };

	std::set< std::pair< size_t, size_t > > visible;

	for( size_t i( 0 ); i != field.size(); ++i )
	{
		{
			char h{ 0 };
			for( size_t j( 0 ); ( j != field[ i ].size() ); ++j )
			{
				if( h < field[ i ][ j ] )
					visible.emplace( i, j );
				h = std::max( h, field[ i ][ j ] );
			}
		}

		{
			char h{ 0 };
			for( size_t j( field[ i ].size() ); j != 0; --j )
			{
				if( h < field[ i ][ j - 1 ] )
					visible.emplace( i, j - 1 );
				h = std::max( h, field[ i ][ j - 1 ] );
			}
		}
	}

	for( size_t j( 0 ); j != field[ 0 ].size(); ++j )
	{
		{
			char h{ 0 };
			for( size_t i( 0 ); ( i != field.size() ); ++i )
			{
				if( h < field[ i ][ j ] )
					visible.emplace( i, j );
				h = std::max( h, field[ i ][ j ] );
			}
		}

		{
			char h{ 0 };
			for( size_t i( field.size() ); i != 0; --i )
			{
				if( h < field[ i - 1 ][ j ] )
					visible.emplace( i - 1, j );
				h = std::max( h, field[ i - 1 ][ j ] );
			}
		}
	}

	std::cout << "result = " << visible.size() << std::endl;

	return 0;
}
