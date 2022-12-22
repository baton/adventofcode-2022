#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>


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

	std::cout << "result = " << monkeys[ "root" ].yell( monkeys ) << std::endl;

	return 0;
}