#include "utils/CommandLineArguments.hpp"
#include "structures/BasicTrie.hpp"

#include <iostream>


int main(int argc, char** argv)
{
  AppCommandLineArguments cmd_line_args( argc, argv );

  BasicTrie trie;

  trie.load( cmd_line_args.path_to_dict_ );

  return 0;
}
