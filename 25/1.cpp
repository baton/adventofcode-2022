#include <iostream>
#include <fstream>
#include <string>


int64_t from_snafu( std::string const & s )
{
	int64_t r{ 0 };
	for( char const c : s )
	{
		r *= 5;
		switch( c )
		{
			case '2': r += 2; break;
			case '1': r += 1; break;
			case '0': r += 0; break;
			case '-': r -= 1; break;
			case '=': r -= 2; break;
			default: break;
		}
	}

	return r;
}

std::string to_snafu( int64_t x )
{
	std::string r;
	while( x != 0 )
	{
		int64_t const d{ x % 5 };
		switch( d )
		{
			case 0: r.push_back( '0' ); x -= d; break;
			case 1: r.push_back( '1' ); x -= d; break;
			case 2: r.push_back( '2' ); x -= d; break;
			case 3: r.push_back( '=' ); x += 2; break;
			case 4: r.push_back( '-' ); x += 1; break;
		}
		x /= 5;
	}
	return { r.crbegin(), r.crend() };
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	int64_t sum{ 0 };

	std::ifstream ifs( argv[ 1 ] );
	while( !ifs.eof() )
	{
		std::string s;
		ifs >> s;

		sum += from_snafu( s );
	}

	std::cout << "result = " << to_snafu( sum ) << std::endl;
}