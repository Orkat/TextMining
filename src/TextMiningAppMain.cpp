#include "utils/CommandLineArguments.hpp"
#include "structures/BasicTrie.hpp"
#include "structures/CompactTrie.hpp"
#include "utils/Functions.hpp"

#include <iostream>


int main(int argc, char** argv)
{
  AppCommandLineArguments cmd_line_args( argc, argv );


  /*
  BasicTrie trie;

  trie.load( cmd_line_args.path_to_dict_ );

  if ( cmd_line_args.print_words_ )
    trie.print_words( std::cout );
  */

  //trie.load_mmap( cmd_line_args.path_to_dict_ );


  //print_file_hex( cmd_line_args.path_to_dict_.c_str(), 200 );



  CompactTrie trie;

  trie.load_mmap( cmd_line_args.path_to_dict_ );
  trie.print_words_mmap();
  


  return 0;
}
