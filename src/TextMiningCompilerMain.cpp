#include "structures/BasicTrie.hpp"
#include "utils/CommandLineArguments.hpp"
#include "utils/WordsContainer.hpp"

#include <iostream>

int main(int argc, char** argv)
{
  CompilerCommandLineArguments cmd_line_args( argc, argv );

  WordsContainer words_container( cmd_line_args.path_to_words_ );

  BasicTrie trie;

  for ( auto iter : words_container.word_frequency_map_ )
    trie.add_word( iter.first, iter.second );

  //trie.print_words_frequencies( std::cout );
  //trie.print_words( std::cout );
  //trie.print( std::cout );
  trie.serialise( cmd_line_args.path_to_dict_ );

  return 0;
}
