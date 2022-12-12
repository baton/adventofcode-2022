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

	size_t max_score{ 0 };
	for( auto const & [ x, y ] : visible )
	{
		if( ( x == 0 ) || ( x == field.size() - 1 )
			|| ( y == 0 ) || ( y == field[ x ].size() - 1 ) )
			continue;

		size_t score{ 1 };

		{
			size_t d{ 0 };
			size_t i{ x + 1 };
			for( ; ( i != field.size() ) && ( field[ i ][ y ] < field[ x ][ y ] ); ++i )
				++d;
			if( i != field.size() )
				++d;
			score *= d;
		}

		{
			size_t d{ 0 };
			size_t i{ x };
			for( ; ( i != 0 ) && ( field[ i - 1 ][ y ] < field[ x ][ y ] ); --i )
				++d;
			if( i != 0 )
				++d;
			score *= d;
		}

		{
			size_t d{ 0 };
			size_t j{ y + 1 };
			for( ; ( j != field[ x ].size() ) && ( field[ x ][ j ] < field[ x ][ y ] ); ++j )
				++d;
			if( j != field[ x ].size() )
				++d;
			score *= d;
		}				

		{
			size_t d{ 0 };
			size_t j{ y };
			for( ; ( j != 0 ) && ( field[ x ][ j - 1 ] < field[ x ][ y ] ); --j )
				++d;
			if( j != 0 )
				++d;
			score *= d;
		}

		if( max_score < score )
			max_score = score;
	}

	std::cout << "result = " << max_score << std::endl;

	return 0;
}
