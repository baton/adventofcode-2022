#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace qi = boost::spirit::qi;


struct valve_info_t
{
	std::string id;
	int rate;
	std::vector< std::string > tunnels;
};

BOOST_FUSION_ADAPT_STRUCT
(
	valve_info_t,
	( std::string, id )
	( int, rate )
	( std::vector< std::string >, tunnels )
);

template< typename Iterator >
struct valve_info_grammar_t : qi::grammar< Iterator, valve_info_t(), qi::space_type >
{
	valve_info_grammar_t()
		: valve_info_grammar_t::base_type( start )
	{
		using qi::lit;
		using qi::char_;

		start %= lit( "Valve" ) >> valve_id >> "has flow rate=" >> qi::int_
			>> "; tunnel" >> *lit( 's' ) >> "lead" >> *lit( 's' ) >> "to valve" >> *lit( 's' ) >> ( valve_id % ',' );
		valve_id = char_( 'A', 'Z' ) >> char_( 'A', 'Z' );
	}

	qi::rule< Iterator, valve_info_t(), qi::space_type > start;
	qi::rule< Iterator, std::string() > valve_id;
};

valve_info_grammar_t< std::string::const_iterator > const valve_info_grammar;

inline int valve_id_to_int( std::string const & vid )
{
	return ( vid[ 0 ] - 'A' ) << 6 | ( vid[ 1 ] - 'A' );
}

std::string int_to_valve_id( int const vid )
{
	std::string res;
	res.push_back( ( ( vid & 0xfc0 ) >> 6 ) + 'A' );
	res.push_back( ( ( vid & 0x3f ) ) + 'A' );
	return res;
}

using valves_t = std::map< int, std::pair< int, std::vector< int > > >;
using valve_set_t = std::set< int >;


struct state_t
{
	valve_set_t opened;
	int pos = 0;
	size_t elapsed = 0;
	size_t released = 0;
};


int main( int argc, char * argv[] )
{
	if( argc < 2 )
		return 0;

	std::ifstream ifs( argv[ 1 ] );

	valves_t valves;
	valve_set_t all_non_zero;

	while( !ifs.eof() )
	{
		std::string s;
		std::getline( ifs, s );

		valve_info_t vi;
		if( !qi::phrase_parse( s.cbegin(), s.cend(), valve_info_grammar, qi::space, vi ) )
			return -1;

		int const vid( valve_id_to_int( vi.id ) );
		if( vi.rate != 0 )
			all_non_zero.emplace( vid );

		valves_t::mapped_type & vv( valves[ vid ] );
		vv.first = vi.rate;
		std::transform(
			vi.tunnels.begin(), vi.tunnels.end(),
			std::back_inserter( vv.second ),
			valve_id_to_int
		);
	}

	for( auto & [ k, v ] : valves )
	{
		std::sort(
			v.second.begin(), v.second.end(),
			[ &valves ]( int const a, int const b ) -> bool
			{
				return valves[ a ].first < valves[ b ].first;
			}
		);
	}

	std::vector< size_t > vertexes;
	vertexes.push_back( valve_id_to_int( "AA" ) );
	vertexes.insert( vertexes.end(), all_non_zero.begin(), all_non_zero.end() );

	std::vector< std::vector< size_t > > distances( vertexes.size(), std::vector< size_t >( vertexes.size(), 0 ) );
	for( size_t i( 0 ); i != distances.size(); ++i )
		for( size_t j( i + 1 ); j != distances[ i ].size(); ++j )
		{
			valve_set_t visited;
			std::list< std::pair< size_t, size_t > > front;
			front.emplace_back( vertexes[ i ], 0 );
			while( front.front().first != vertexes[ j ] )
			{
				std::pair< size_t, size_t > p( front.front() );
				front.pop_front();
				visited.insert( p.first );
				for( int const v : valves[ p.first ].second )
					if( !visited.count( v ) )
						front.emplace_back( v, p.second + 1 );
			}

			distances[ i ][ j ] = front.front().second;
			distances[ j ][ i ] = front.front().second;
		}

	std::list< state_t > front;
	front.push_back( { { 0 }, 0, 0, 0 } );
	size_t max_released{ 0 };

	std::list< state_t > db;

	while( !front.empty() )
	{
		state_t s( front.front() );
		front.pop_front();

		for( size_t i( 1 ); i != vertexes.size(); ++i )
			if( !s.opened.count( i )
				&& ( ( s.elapsed + distances[ s.pos ][ i ] + 1 ) <= 26 ) )
			{
				state_t sn( s );
				sn.elapsed += distances[ sn.pos ][ i ] + 1;
				for( int const vi : sn.opened )
					sn.released += valves[ vertexes[ vi ] ].first * ( distances[ sn.pos ][ i ] + 1 );
				sn.opened.insert( i );
				sn.pos = i;

				front.push_front( std::move( sn ) );
			}

		for( ; s.elapsed != 26; ++s.elapsed )
			for( int const vi : s.opened )
				s.released += valves[ vertexes[ vi ] ].first;

		db.push_back( std::move( s ) );
	}

	for( std::list< state_t >::const_iterator i( db.begin() ); i != db.end(); ++i )
		for( std::list< state_t >::const_iterator j( std::next( i ) ); j != db.end(); ++j )
		{
			bool skip{ false };
			for( int const x : i->opened )
				if( ( x != 0 ) && j->opened.count( x ) )
				{
					skip = true;
					break;
				}
			if( skip )
				continue;
			for( int const x : j->opened )
				if( ( x != 0 ) && i->opened.count( x ) )
				{
					skip = true;
					break;
				}
			if( skip )
				continue;

			size_t const released{ i->released + j->released };
			if( max_released < released )
				max_released = released;
		}


	std::cout << "result = " << max_released << std::endl;

	return 0;
}