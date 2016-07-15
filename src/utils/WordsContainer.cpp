#include "utils/WordsContainer.hpp"

#include <iostream>
#include <fstream>

WordsContainer::WordsContainer()
{

}

WordsContainer::WordsContainer( const std::string& path_to_words )
{
  load_path( path_to_words );
}

void WordsContainer::load_path( const std::string& path_to_words )
{
  std::ifstream words_file(path_to_words);

  std::string line;
  while ( std::getline( words_file, line ) )
  {
    unsigned int token_index = line.find( '\t' );
    std::string str_a = line.substr( 0, token_index );
    std::string str_b = line.substr( token_index + 1, line.size() );
    unsigned int frequency = std::stoi(str_b);

    word_frequency_map_[str_a] = frequency;
  }
}
