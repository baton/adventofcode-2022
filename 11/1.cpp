#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <cmath>


using namespace std::string_view_literals;


enum op_t { op_add, op_mul, op_sqr };


struct monkey_t
{
	std::vector< uint64_t > items;
	op_t op;
	uint64_t op_val;
	uint64_t test_factor;
	uint64_t if_true, if_false;
};


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	std::vector< monkey_t > monkeys;

	while( !ifs.eof() )
	{
		monkey_t monkey;

		std::string new_monkey;
		std::getline( ifs, new_monkey );

		std::string items;
		std::getline( ifs, items );
		{
			char const * b( items.c_str() + "  Starting items: "sv.length() );
			char const * e( items.c_str() + items.length() );
			while( b != e )
			{
				char * ee;
				uint64_t const i( std::strtoull( b, &ee, 10 ) );
				monkey.items.push_back( i );
				b = ee;
				if( b != e )
				{
					if( *b == ',' )
						++b;
					if( *b == ' ' )
						++b;
				}
			}
		}

		std::string operation;
		std::getline( ifs, operation );
		constexpr size_t const op_pos{ "  Operation: new = old "sv.length() };
		switch( operation[ op_pos ] )
		{
			case '+': monkey.op = op_add; break;
			case '*': monkey.op = operation[ op_pos + 2 ] == 'o' ? op_sqr : op_mul; break;
			default: break;
		}
		if( monkey.op != op_sqr )
			monkey.op_val = std::strtoull( operation.c_str() + op_pos + 2, nullptr, 10 );

		std::string test;
		std::getline( ifs, test );
		monkey.test_factor = std::strtoull( test.c_str() + "  Test: divisible by "sv.length(), nullptr, 10 );

		std::string if_true;
		std::getline( ifs, if_true );
		monkey.if_true = std::strtoull( if_true.c_str() + "    If true: throw to monkey "sv.length(), nullptr, 10 );

		std::string if_false;
		std::getline( ifs, if_false );
		monkey.if_false = std::strtoull( if_false.c_str() + "    If false: throw to monkey "sv.length(), nullptr, 10 );

		if( !ifs.eof() )
		{
			std::string t;
			std::getline( ifs, t );
		}

		monkeys.push_back( std::move( monkey ) );
	}

	std::vector< uint64_t > counts( monkeys.size(), 0ULL );

	for( size_t round( 0 ); round != 20; ++round )
	{
		for( size_t m( 0 ); m != monkeys.size(); ++m )
		{
			monkey_t & monkey( monkeys[ m ] );
			counts[ m ] += monkey.items.size();

			for( uint64_t i : monkey.items )
			{
				switch( monkey.op )
				{
					case op_add: i += monkey.op_val; break;
					case op_mul: i *= monkey.op_val; break;
					case op_sqr: i *= i; break;
				}
				i = static_cast< uint64_t >( floor( i / 3 ) );
				monkeys[ i % monkey.test_factor == 0 ? monkey.if_true : monkey.if_false ].items.push_back( i );
			}
			monkey.items.clear();
		}
	}

	std::sort( counts.begin(), counts.end(), std::greater< uint64_t >{} );

	std::cout << "result = " << ( counts[ 0 ] * counts[ 1 ] ) << std::endl;

	return 0;
}