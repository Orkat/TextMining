#include <string>

struct CompactTrieNodeList;

struct CompactTrieNode
{
  char value_;
  unsigned int frequency_;
  CompactTrieNodeList* children_;
};

struct CompactTrieNodeList
{
  CompactTrieNodeList* next_;
  CompactTrieNode* node_;
};


class CompactTrie
{

  public:

    CompactTrie( void );
    void add_word( const std::string& word, unsigned int frequency );

  private:

    CompactTrieNode* root_node_;

    void add_word_aux( CompactTrieNode* compact_trie_node, const char* word, unsigned int frequency );

};

CompactTrieNode* add_child_node( CompactTrieNode* parent, char value );
CompactTrieNode* find_node( CompactTrieNodeList* list, char value );
CompactTrieNode* append_node( CompactTrieNodeList* list, char value );
CompactTrieNode* insert_node_lower_bound( CompactTrieNodeList* list, char value ); // insert in correct order
