#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <limits>
#include <algorithm>


enum op_t
{
	op_none, op_add, op_sub, op_mul, op_div
};


struct monkey_t
{
	op_t op = op_none;
	int64_t n = 0;
	std::string m1, m2;

	int64_t yell( std::map< std::string, monkey_t > const & monkeys ) const
	{
		if( op == op_none )
			return n;

		int64_t const a( monkeys.at( m1 ).yell( monkeys ) );
		int64_t const b( monkeys.at( m2 ).yell( monkeys ) );
		switch( op )
		{
			case op_add: return a + b;
			case op_sub: return a - b;
			case op_mul: return a * b;
			case op_div: return a / b;

			default: return 0;
		}
	}
};


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::map< std::string, monkey_t > monkeys;

	std::ifstream ifs( argv[ 1 ] );
	while( !ifs.eof() )
	{
		std::string mn, ma;
		ifs >> mn >> ma;
		mn.pop_back();

		monkey_t m;
		if( std::isdigit( ma[ 0 ] ) )
			m.n = std::stoll( ma );
		else
		{
			m.m1 = std::move( ma );
			std::string mo;
			ifs >> mo >> m.m2;
			switch( mo[ 0 ] )
			{
				case '+': m.op = op_add; break;
				case '-': m.op = op_sub; break;
				case '*': m.op = op_mul; break;
				case '/': m.op = op_div; break;

				default: return -1;
			}
		}

		monkeys.emplace( std::move( mn ), std::move( m ) );
	}

	monkey_t const & ma( monkeys[ monkeys[ "root" ].m1 ] );
	monkey_t const & mb( monkeys[ monkeys[ "root" ].m2 ] );
	monkey_t & humn( monkeys.at( "humn" ) );

	int64_t const ayo{ ma.yell( monkeys ) };
	humn.n = 0;
	int64_t const ay0{ ma.yell( monkeys ) };

	monkey_t const & mc{ ayo == ay0 ? ma : mb };
	int64_t const mcy{ mc.yell( monkeys ) };
	monkey_t const & md{ ayo == ay0 ? mb : ma };

	int64_t a{ 0 }, b{ std::numeric_limits< int64_t >::max() >> 1 };
	int64_t xa, xb;
	while( ( a + 1 ) != b )
	{
		humn.n = a;
		xa = md.yell( monkeys );
		humn.n = b;
		xb = md.yell( monkeys );

		humn.n = ( a + b ) >> 1;
		int64_t const xc{ md.yell( monkeys ) };

		int64_t const mn{ std::min( xa, xc ) };
		int64_t const mx{ std::max( xa, xc ) };

		if( ( mcy >= mn ) && ( mcy <= mx ) )
			b = humn.n;
		else
			a = humn.n;
	}

	std::cout << "result = " << ( xa == mcy ? a : b )  << std::endl;

	return 0;
}