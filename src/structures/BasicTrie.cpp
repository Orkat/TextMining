#include "structures/BasicTrie.hpp"
#include "utils/Functions.hpp"
#include "utils/Defines.hpp"

#include <fstream>
#include <iostream>

BasicTrie::BasicTrie()
{
  frequency_ = 0;
}

BasicTrie::BasicTrie( const std::string& word, unsigned int frequency )
{
  frequency_ = 0;
  if ( !word.empty() )
    add_word( word , frequency );
  else
    frequency_ = frequency;
}

void BasicTrie::add_word( const std::string& word , unsigned int frequency )
{
  if ( !word.empty() )
  {
    auto word_cpy = word;
    word_cpy.erase( word_cpy.begin() );

    char c = word.at( 0 );
    if ( children_.find( c ) == children_.end() )
      children_[ c ] = new BasicTrie( word_cpy, frequency );
    else
      children_[ c ]->add_word( word_cpy, frequency );
  }
  else
  {
    frequency_ = frequency;
  }
}

// root node is not written : need auxiliary function to distinguish
// children of root node from other cases
void BasicTrie::serialise( const std::string& filename )
{
  set_address( 0 );
  std::ofstream file( filename , std::ios::out | std::ios::binary );
  write_binary_unsigned_int( file, children_.size(), BASIC_TRIE_N_CHILDREN_SIZE );
  for ( auto iter : children_ )
    iter.second->serialise_aux( file, iter.first );
  file.close();
}

void BasicTrie::serialise_aux( std::ofstream& file , char value )
{
  file.write( &value, BASIC_TRIE_VALUE_SIZE );

  write_binary_unsigned_int( file, address_, BASIC_TRIE_ADDRESS_SIZE );
  write_binary_unsigned_int( file, frequency_, BASIC_TRIE_FREQUENCY_SIZE );
  write_binary_unsigned_int( file, children_.size(), BASIC_TRIE_N_CHILDREN_SIZE );

  for ( auto iter : children_ )
    iter.second->serialise_aux( file, iter.first );
}

void BasicTrie::load( const std::string& filename )
{

}

// root node is not written : need auxiliary function to distinguish
// children of root node from other cases
void BasicTrie::set_address( unsigned int address )
{
  for ( auto iter : children_ )
  {
    address = iter.second->set_address_aux( address );
    address += BASIC_TRIE_VALUE_SIZE + BASIC_TRIE_ADDRESS_SIZE + BASIC_TRIE_FREQUENCY_SIZE;
  }
}

unsigned int BasicTrie::set_address_aux( unsigned int address )
{
  address_ = address;
  for ( auto iter : children_ )
  {
    address += BASIC_TRIE_VALUE_SIZE + BASIC_TRIE_ADDRESS_SIZE + BASIC_TRIE_FREQUENCY_SIZE;
    address = iter.second->set_address_aux( address );
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




// --------------- PRINTING FUNCTIONS ------------------ //

void BasicTrie::print( std::ostream& out )
{
  print_aux( out , std::string() );
}

void BasicTrie::print_words( std::ostream& out )
{
  print_words_aux( out, std::string() );
  out << std::flush;
}

void BasicTrie::print_words_frequencies( std::ostream& out )
{
  print_words_frequencies_aux( out , std::string() );
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

void BasicTrie::print_words_aux( std::ostream& out, std::string buf )
{
  if ( children_.empty() )
    out << buf << "\n";
  else
  {
    for ( auto iter : children_ )
    {
      std::string new_buf = buf;
      new_buf.push_back( iter.first );
      iter.second->print_words_aux( out , new_buf );
    }
  }
}

void BasicTrie::print_words_frequencies_aux( std::ostream& out , std::string buf )
{
  if ( children_.empty() || frequency_ != 0 )
  {
    out << buf << "\t" << frequency_ << "\n";
  }
  for ( auto iter : children_ )
  {
    std::string new_buf = buf;
    new_buf.push_back( iter.first );
    iter.second->print_words_frequencies_aux( out , new_buf );
  }
}

// ------------------------------------------- //
