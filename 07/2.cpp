#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <memory>


using file_t = std::pair< std::string, uint64_t >;

struct dir_t
{
	dir_t( dir_t * p, std::string const & n )
		: up( p )
		, name( n )
	{}

	dir_t * up = nullptr;
	std::string const name;
	std::map< std::string, dir_t * > subs;
	std::list< file_t > files;
	uint64_t size = 0;
};

using fs_t = std::list< dir_t >;

dir_t * cd( fs_t & fs, dir_t * current, std::string const & name )
{
	if( name == ".." )
		return current->up;

	std::map< std::string, dir_t * >::const_iterator i( current->subs.find( name ) );
	if( i != current->subs.end() )
		return i->second;

	fs.emplace_back( current, name );
	return current->subs.emplace( name, &fs.back() ).first->second;
}

uint64_t calc_sizes( dir_t * dir )
{
	dir->size =
		std::transform_reduce(
			dir->files.begin(), dir->files.end(),
			0ULL,
			std::plus{},
			[]( file_t const & f ) -> uint64_t
			{
				return f.second;
			}
		)
		+
		std::transform_reduce(
			dir->subs.begin(), dir->subs.end(),
			0ULL,
			std::plus{},
			[]( auto & dd ) -> uint64_t
			{
				return calc_sizes( dd.second );
			}
		);

	return dir->size;
}


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	fs_t fs;
	fs.emplace_back( nullptr, "/" );
	dir_t * root( &fs.back() );
	dir_t * curdir( root );

	std::ifstream ifs( argv[ 1 ] );
	while( !ifs.eof() )
	{
		std::string cmd;
		std::getline( ifs, cmd );

		if( cmd[ 0 ] == '$' )
		{
			if( cmd.substr( 0, 4 ) == "$ cd" )
				curdir = cd( fs, curdir, cmd.substr( 5 ) );
		}
		else
		{
			if( cmd.substr( 0, 3 ) != "dir" )
			{
				char const * b( cmd.c_str() );
				char * e( nullptr );
				uint64_t const x( std::strtoull( b, &e, 10 ) );
				++e;

				curdir->files.emplace_back( e, x );
			}
		}
	}

	uint64_t total( calc_sizes( root ) );

	constexpr uint64_t const fullsize{ 70000000ULL };
	constexpr uint64_t const needfree{ 30000000ULL };

	uint64_t const needed( needfree - ( fullsize - total ) );

	std::vector< uint64_t > sizes;
	std::transform(
		fs.begin(), fs.end(),
		std::back_inserter( sizes ),
		[]( dir_t const & d ) -> uint64_t
		{
			return d.size;
		}
	);
	std::sort( sizes.begin(), sizes.end() );

	uint64_t const result{
		*std::upper_bound( sizes.begin(), sizes.end(), needed )
	};

	std::cout << "result = " << result << std::endl;

	return 0;
}