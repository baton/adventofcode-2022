#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include <cstdlib>


using pos_t = std::pair< int64_t, int64_t >;
using map_t = std::map< pos_t, char >;

enum dir_t : int { right, down, left, up };


inline constexpr dir_t turn( dir_t const d, char const c )
{
	if( ( c == 'L' ) && ( d == right ) )
		return up;
	else if( ( c == 'R' ) && ( d == up ) )
		return right;
	else if( c == 'L' )
		return static_cast< dir_t >( d - 1 );
	else
		return static_cast< dir_t >( d + 1 );
}

static_assert( turn( right, 'L' ) == up, "" );
static_assert( turn( down,  'L' ) == right, "" );
static_assert( turn( left,  'L' ) == down, "" );
static_assert( turn( up,    'L' ) == left, "" );
static_assert( turn( right, 'R' ) == down, "" );
static_assert( turn( down,  'R' ) == left, "" );
static_assert( turn( left,  'R' ) == up, "" );
static_assert( turn( up,    'R' ) == right, "" );

inline pos_t move_dir( pos_t const & p, dir_t const d )
{
	switch( d )
	{
		case up:    return { p.first,     p.second - 1 };
		case down:  return { p.first,     p.second + 1 };
		case left:  return { p.first - 1, p.second     };
		case right: return { p.first + 1, p.second     };
	}

	return p;
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	map_t m;
	int64_t y{ 0 };
	pos_t start{ -1, -1 };

	std::ifstream ifs( argv[ 1 ] );
	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );

		if( s.empty() )
			break;

		for( int64_t x{ 0 }; x != s.length(); ++x )
		{
			char const c{ s[ x ] };
			if( c != ' ' )
			{
				m.insert( { { x, y }, c } );
				if( start.first == -1 )
					start = { x, y };
			}
		}

		++y;
	}

	std::string cmds;
	ifs >> cmds;

	pos_t p{ start };
	dir_t d{ right };

	char const * b{ cmds.c_str() };
	char const * e{ b + cmds.length() };
	while( b != e )
	{
		char * s;
		int64_t const l{ std::strtoll( b, &s, 10 ) };
		int64_t i( 0 );
		for( ; i != l; ++i )
		{
			pos_t pp{ move_dir( p, d ) };
			map_t::const_iterator it{ m.find( pp ) };
			if( it != m.end() )
			{
				if( it->second == '#' )
					break;

				p = std::move( pp );
			}
			else
			{
				dir_t dd{ turn( turn( d, 'R' ), 'R' ) };
				for( pp = p; m.count( pp ); pp = move_dir( pp, dd ) ) {}
				pp = move_dir( pp, d );
				if( m.at( pp ) == '#' )
					break;

				p = std::move( pp );
			}
		}

		if( s == e )
			break;

		d = turn( d, *s );
		b = s + 1;
	}

	int64_t const result{ ( p.second + 1 ) * 1000 + ( p.first + 1 ) * 4 + static_cast< int >( d ) };

	std::cout << "result = " << result << std::endl;

	return 0;
}