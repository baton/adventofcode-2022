#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <numeric>
#include <algorithm>
#include <iterator>


struct line_t : std::string
{};

std::istream & operator>>( std::istream & is, line_t & line )
{
	return std::getline( is, line );
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::list< std::string > data;
	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );
		if( s.empty() )
			break;

		data.push_front( std::move( s ) );
	}

	size_t const cols( ( data.front().length() + 1 ) / 4 );
	data.pop_front();

	std::vector< std::vector< char > > v( cols, std::vector< char >{} );
	for( std::string const & s : data )
	{
		for( size_t i( 0 ); i != cols; ++i )
		{
			char const c( s[ i * 4 + 1 ] );
			if( c != ' ' )
				v[ i ].push_back( c );
		}
	}

	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );

		unsigned int c{ 0 }, f{ 0 }, t{ 0 };
		std::sscanf( s.c_str(), "move %d from %d to %d", &c, &f, &t );
		--f;
		--t;

		v[ t ].insert( v[ t ].end(), v[ f ].end() - c, v[ f ].end() );
		v[ f ].erase( v[ f ].end() - c, v[ f ].end() );
	}

	std::string const result{
		std::accumulate(
			v.begin(), v.end(),
			std::string{},
			[]( std::string & s, std::vector< char > const & v ) -> std::string
			{
				s.push_back( v.back() );
				return std::move( s );
			}
		)
	};

	std::cout << "result = " << result << std::endl;

	return 0;
}