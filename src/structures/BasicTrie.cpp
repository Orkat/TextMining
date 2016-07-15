#include "structures/BasicTrie.hpp"
#include "utils/functions.hpp"
#include "utils/defines.hpp"

#include <fstream>
#include <iostream>

BasicTrie::BasicTrie()
{

}

BasicTrie::BasicTrie( const std::string& word )
{
  if ( !word.empty() )
    add_word( word );
}

void BasicTrie::add_word( const std::string& word )
{
  auto word_cpy = word;
  word_cpy.erase( word_cpy.begin() );

  char c = word.at( 0 );
  if ( children_.find( c ) == children_.end() )
    children_[ c ] = new BasicTrie( word_cpy );
  else
    children_[ c ]->add_word( word_cpy );
}

void BasicTrie::print( std::ostream& out )
{
  print_aux( out , std::string() );
}

void BasicTrie::print_aux( std::ostream& out , std::string buf )
{
  auto new_buf = buf;

  if (new_buf.size() >= 2 )
  {
    new_buf = new_buf.substr(0, new_buf.size() - 1 );
    new_buf.push_back(' ');
  }
  new_buf.push_back('|');
  new_buf.push_back('-');

  for ( auto iter : children_ )
  {
    out << buf << iter.first << "\n";
    iter.second->print_aux( out , new_buf );
  }
}

void BasicTrie::serialise( const std::string& filename )
{
  unsigned int n_bytes_per_value = BASIC_TRIE_VALUE_SIZE;
  unsigned int n_bytes_per_address = BASIC_TRIE_ADDRESS_SIZE;

  set_address( 0, n_bytes_per_value, n_bytes_per_address );

  std::ofstream file( filename , std::ios::out | std::ios::binary );

  for ( auto iter : children_ )
  {
    iter.second->serialise_aux( file, iter.first, n_bytes_per_value, n_bytes_per_address );
  }
  file.close();
}

void BasicTrie::serialise_aux( std::ofstream& file , char value, unsigned int n_bytes_per_value, unsigned int n_bytes_per_address )
{
  file.write( &value, 1 );

  char* buffer = new char[ n_bytes_per_address ];
  set_unsigned_int_buffer( address_, buffer, n_bytes_per_address );
  file.write( buffer, n_bytes_per_address );
  delete buffer;

  for ( auto iter : children_ )
  {
    iter.second->serialise_aux( file, iter.first, n_bytes_per_value, n_bytes_per_address );
  }
}

// root node is not written : need auxiliary function to distinguish
// children of root node from other cases
void BasicTrie::set_address( unsigned int address, unsigned int n_bytes_per_value, unsigned int n_bytes_per_address )
{
  for ( auto iter : children_ )
  {
    address = iter.second->set_address_aux( address, n_bytes_per_value, n_bytes_per_address );
    address += n_bytes_per_value + n_bytes_per_address;
  }
}

unsigned int BasicTrie::set_address_aux( unsigned int address, unsigned int n_bytes_per_value, unsigned int n_bytes_per_address )
{
  address_ = address;
  for ( auto iter : children_ )
  {
    address += n_bytes_per_value + n_bytes_per_address;
    address = iter.second->set_address_aux( address, n_bytes_per_value, n_bytes_per_address );
  }
  return address;
}

unsigned int BasicTrie::count_nodes( void )
{
  unsigned int n = 1;
  for ( auto iter : children_ )
    n += iter.second->count_nodes();
  return n;
}
