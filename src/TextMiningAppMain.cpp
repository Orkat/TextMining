#include "utils/CommandLineArguments.hpp"
#include "structures/BasicTrie.hpp"
#include "structures/CompactTrie.hpp"
#include "utils/Functions.hpp"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
  AppCommandLineArguments cmd_line_args( argc, argv );

  CompactTrie trie;

  trie.load_mmap( cmd_line_args.path_to_dict_ );

  std::vector<std::string> lines;

  std::string line;
  while ( std::getline(std::cin, line) )
  {
    std::size_t found = line.find("\\n");
    while ( found != std::string::npos )
    {
      lines.push_back( line.substr(0, found) );
      line = line.substr( found+2, line.size() - found - 1 );
      found = line.find("\\n");
    }
    lines.push_back( line );
  }

  for ( unsigned int i = 0; i < lines.size(); ++i )
  {
    while( lines[i].at(0) == ' ' )
      lines[i] = lines[i].erase(0, 1);
  }

  for ( auto line : lines )
  {
    std::string str1, str2, str3;

    std::string str = line;
    std::size_t found;

    found = str.find(" ");
    str1 = str.substr(0, found);
    str = str.substr(found+1, line.size() - found - 1);

    found = str.find(" ");
    str2 = str.substr(0, found);
    str = str.substr(found+1, line.size() - found - 1);

    found = str.find(" ");
    str3 = str.substr(0, found);

    int distance = std::stoi(str2);
    std::string word = str3;

    auto dl_words = trie.get_dlwords( word, distance );
    sort_dl_words( dl_words );

    std::cout << "[";
    for ( unsigned int i = 0; i < dl_words.size(); ++i )
    {
      auto word = dl_words[i];
      std::cout << "{\"word\":\"" << std::get<0>(word) << "\",\"freq\":" << std::get<2>(word) << ",\"distance\":";
      std::cout << std::get<1>(word) << "}";
      if ( i < dl_words.size() - 1 )
        std::cout << ",";
    }
    std::cout << "]" << std::endl;
  }

  return 0;
}
