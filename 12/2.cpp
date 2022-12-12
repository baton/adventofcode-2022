#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <algorithm>


using point_t = std::pair< uint64_t, uint64_t >;


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::vector< std::string > field{ std::istream_iterator< std::string >( ifs ),
		std::istream_iterator< std::string >() };

	point_t e;
	std::vector< point_t > starts;
	for( uint64_t j( 0 ); j != field.size(); ++j )
		for( uint64_t i( 0 ); i != field[ j ].size(); ++i )
		{
			switch( field[ j ][ i ] )
			{
				case 'E': e = { i, j }; field[ j ][ i ] = 'z'; break;
				case 'S': field[ j ][ i ] = 'a';
				case 'a': starts.emplace_back( i, j );
				default: break;
			}
		}

	std::vector< uint64_t > paths;

	for( point_t const & s : starts )
	{
		std::list< point_t > front;
		std::map< point_t, uint64_t > lengths;

		front.push_back( s );
		lengths[ s ] = 0;

		while( !lengths.count( e ) && !front.empty() )
		{
			point_t p( front.front() );
			uint64_t l( lengths[ p ] );
			front.pop_front();

			if( ( p.first > 0 ) && !lengths.count( { p.first - 1, p.second } )
				&& ( field[ p.second ][ p.first - 1 ] <= field[ p.second ][ p.first ] + 1 ) )
			{
				front.emplace_back( p.first - 1, p.second );
				lengths[ front.back() ] = l + 1;
			}
			if( ( p.first < field[ 0 ].size() - 1 ) && !lengths.count( { p.first + 1, p.second } )
				&& ( field[ p.second ][ p.first + 1 ] <= field[ p.second ][ p.first ] + 1 ) )
			{
				front.emplace_back( p.first + 1, p.second );
				lengths[ front.back() ] = l + 1;
			}
			if( ( p.second > 0 ) && !lengths.count( { p.first, p.second - 1 } )
				&& ( field[ p.second - 1 ][ p.first ] <= field[ p.second ][ p.first ] + 1 ) )
			{
				front.emplace_back( p.first, p.second - 1 );
				lengths[ front.back() ] = l + 1;
			}
			if( ( p.second < field.size() - 1 ) && !lengths.count( { p.first, p.second + 1 } )
				&& ( field[ p.second + 1 ][ p.first ] <= field[ p.second ][ p.first ] + 1 ) )
			{
				front.emplace_back( p.first, p.second + 1 );
				lengths[ front.back() ] = l + 1;
			}
		}

		if( lengths.count( e ) )
			paths.push_back( lengths[ e ] );
	}

	std::sort( paths.begin(), paths.end() );

	std::cout << "result = " << paths.front() << std::endl;

	return 0;
}