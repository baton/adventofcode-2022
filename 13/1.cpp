#include <iostream>
#include <fstream>
#include <string>
#include <cctype>


int cmp( char const * & l, char const * & r )
{
	while( *l && *r && ( *l != ']' ) && ( *r != ']' ) )
	{
		if( std::isdigit( *l ) && std::isdigit( *r ) )
		{
			char * le;
			char * re;

			long const ll( std::strtol( l, &le, 10 ) );
			long const rl( std::strtol( r, &re, 10 ) );

			if( ll < rl )
				return -1;
			if( rl < ll )
				return 1;

			l = le;
			r = re;
		}
		else if( ( *l == '[' ) && ( *r == '[' ) )
		{
			++l;
			++r;

			int const c( cmp( l, r ) );
			if( c != 0 )
				return c;

			++l;
			++r;
		}
		else if( *l == '[' )
		{
			++l;
			std::string s;
			while( std::isdigit( *r ) )
				s.push_back( *r++ );
			s.push_back( ']' );
			char const * p( s.c_str() );

			int const c( cmp( l, p ) );
			if( c != 0 )
				return c;

			++l;
		}
		else if( *r == '[' )
		{
			std::string s;
			while( std::isdigit( *l ) )
				s.push_back( *l++ );
			s.push_back( ']' );
			char const * p( s.c_str() );
			++r;

			int const c( cmp( p, r ) );
			if( c != 0 )
				return c;

			++r;
		}

		if( ( *l == ',' ) && ( *r == ',' ) )
		{
			++l;
			++r;
		}
	}

	if( ( *l == ']' ) && ( *r == ']' ) )
	{
		return 0;
	}
	else if( *l == ']' )
	{
		return -1;
	}
	else if( *r == ']' )
	{
		return 1;
	}

	return 0;
}


bool is_less( std::string const & l, std::string const & r )
{
	char const * lp( l.c_str() + 1 );
	char const * rp( r.c_str() + 1 );
	return cmp( lp, rp ) < 0;
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	size_t index{ 1 };
	size_t sum{ 0 };

	while( !ifs.eof() )
	{
		std::string left, right;
		ifs >> left >> right;

		if( is_less( left, right ) )
			sum += index;

		++index;
	}

	std::cout << "result = " << sum << std::endl;

	return 0;
}