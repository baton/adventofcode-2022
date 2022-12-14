#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <memory>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

namespace qi = boost::spirit::qi;


using point_t = std::pair< int, int >;


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

	std::set< point_t > cave;

	int max_y{ 0 };

	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );

		std::vector< point_t > v;
		if( !qi::phrase_parse( s.begin(), s.end(), ( qi::int_ >> ',' >> qi::int_ ) % "->", qi::space, v ) )
			return -1;

		for( size_t i( 0 ); i != v.size() - 1; ++i )
		{
			point_t const & a( v[ i ] );
			point_t const & b( v[ i + 1 ] );

			if( a.first == b.first )
			{
				for( int y( a.second ), dy( signum( b.second - a.second ) ); y != b.second + dy; y += dy )
					cave.emplace( a.first, y );
			}
			else if( a.second == b.second )
			{
				for( int x( a.first ), dx( signum( b.first - a.first ) ); x != b.first + dx; x += dx )
					cave.emplace( x, a.second );
			}
		}

		for( point_t const & p : v )
		{
			if( max_y < p.second )
				max_y = p.second;
		}
	}

	size_t count{ 0 };
	bool full{ false };

	while( !full )
	{
		int x{ 500 }, y{ 0 };

		bool touchdown{ false };

		while( !touchdown )
		{
			if( !cave.count( { x, y + 1 } ) )
				++y;
			else if( !cave.count( { x - 1, y + 1 } ) )
				--x, ++y;
			else if( !cave.count( { x + 1, y + 1 } ) )
				++x, ++y;
			else
				touchdown = true;

			touchdown |= ( y == max_y + 1 );
		}

		if( touchdown )
		{
			cave.emplace( x, y );

			++count;
		}

		full = ( x == 500 ) && ( y == 0 );
	}

	std::cout << "result = " << count << std::endl;

	return 0;
}             