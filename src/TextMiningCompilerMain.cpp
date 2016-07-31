#include "structures/BasicTrie.hpp"
#include "structures/CompactTrie.hpp"
#include "utils/CommandLineArguments.hpp"
#include "utils/WordsContainer.hpp"
#include "utils/WordsIterator.hpp"

#include <iostream>

int main(int argc, char** argv)
{
  CompilerCommandLineArguments cmd_line_args( argc, argv );


  WordsIterator words_iterator( cmd_line_args.path_to_words_ );

  CompactTrie trie;

  auto iter = words_iterator.get_next();
  while ( iter != nullptr )
  {
    trie.add_word( iter->first, iter->second );
    auto iter_cpy = iter;
    iter = words_iterator.get_next();
    delete iter_cpy;
  }

  //trie.print_words();

  trie.serialise( cmd_line_args.path_to_dict_ );

  return 0;
}
