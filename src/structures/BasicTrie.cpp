#include "structures/BasicTrie.hpp"


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
