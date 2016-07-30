#include "structures/TrieBuilder.hpp"


int main(int argc, char** argv) {

  Trie* trie = new Trie(argv[1]);
  std::string s0("");
  trie->printWords(s0);
  return 0;

}
