#include "utils/WordsIterator.hpp"

#include <iostream>
#include <fstream>

WordsIterator::WordsIterator()
{

}

WordsIterator::WordsIterator( const std::string& path_to_words )
{
  load_path( path_to_words );
}

void WordsIterator::load_path( const std::string& path_to_words )
{
  words_file_ = std::ifstream(path_to_words);
}

std::pair<std::string, unsigned int>* WordsIterator::get_next()
{
  std::string line;
  if ( std::getline( words_file_, line ) )
  {
    unsigned int token_index = line.find( '\t' );
    std::string str_a = line.substr( 0, token_index );
    std::string str_b = line.substr( token_index + 1, line.size() );
    unsigned int frequency = std::stoi(str_b);

    std::pair<std::string, unsigned int> ret_value = std::make_pair(str_a, frequency);
    std::pair<std::string, unsigned int>* ret = new std::pair<std::string, unsigned int>(ret_value);

    return ret;
  }
  return nullptr;
}
