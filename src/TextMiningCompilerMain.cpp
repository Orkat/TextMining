#include "structures/BasicTrie.hpp"
#include "structures/CompactTrie.hpp"
#include "utils/CommandLineArguments.hpp"
#include "utils/WordsContainer.hpp"
#include "utils/WordsIterator.hpp"

#include <iostream>

int main(int argc, char** argv)
{
  CompilerCommandLineArguments cmd_line_args( argc, argv );

  /*
  WordsContainer words_container( cmd_line_args.path_to_words_ );

  BasicTrie trie;

  for ( auto iter : words_container.word_frequency_map_ )
    trie.add_word( iter.first, iter.second );
  */

  WordsIterator words_iterator( cmd_line_args.path_to_words_ );

  CompactTrie trie;

  //std::cout << "adding words" << std::endl;
  auto iter = words_iterator.get_next();
  while ( iter != nullptr )
  {
    trie.add_word( iter->first, iter->second );
    auto iter_cpy = iter;
    iter = words_iterator.get_next();
    delete iter_cpy;
  }

  //trie.print_words();
  //std::cout << "finished adding words" << std::endl;
  //std::cout << "saving file" << std::endl;
  trie.serialise( cmd_line_args.path_to_dict_ );

  return 0;
}
