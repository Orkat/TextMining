#include "structures/TrieBuilder.hpp"
#include "utils/CommandLineArguments.hpp"
#include "utils/WordsContainer.hpp"

#include <iostream>

int main(int argc, char** argv)
{
  CompilerCommandLineArguments cmd_line_args( argc, argv );

  WordsContainer words_container( cmd_line_args.path_to_words_ );

  TrieBuilder trieRoot;

  for ( auto iter : words_container.word_frequency_map_ )
    trieRoot.addWord( iter.first, iter.second );

  std::string s0 ("");
  trieRoot.printWords(s0);

  // FIXME: Craving RAM and crappy implementation
  //trieRoot.compress(s0, &trieRoot, false);
  trieRoot.serialize( cmd_line_args.path_to_dict_ );

  return 0;
}
