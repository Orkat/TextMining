#include <string>
#include <utility>
#include <sys/mman.h>
#include <vector>

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
    void serialise( const std::string& filename );
    void load_mmap( const std::string& filename );
    void print_words( void );
    void print_words_mmap( void );
    std::vector<std::string> get_dlwords( std::string word, unsigned int distance );

  private:

    CompactTrieNode* root_node_;
    void* file_mmap_;

    void add_word_aux( CompactTrieNode* node, const char* word, unsigned int frequency );
    unsigned int serialise_aux( std::ofstream& file, const CompactTrieNode* node, unsigned int current_offset );
    void serialise_children_list( std::ofstream& file, const CompactTrieNodeList* children );
    std::pair<bool, unsigned int> get_children_offset( const CompactTrieNode* current_node, const CompactTrieNode* node );
    void print_words_aux( const CompactTrieNode* node, std::string word );
    void print_words_mmap_aux( unsigned int offset, std::string word );
    std::vector<std::string> get_dlwords_aux( unsigned int offset, std::string current_word, std::string word,
                                              unsigned int distance );

};

CompactTrieNode* add_child_node( CompactTrieNode* parent, char value );
CompactTrieNode* find_node( CompactTrieNodeList* list, char value );
CompactTrieNode* append_node( CompactTrieNodeList* list, char value );
CompactTrieNode* insert_node_lower_bound( CompactTrieNodeList* list, char value ); // insert in correct order
unsigned int calculate_list_size( CompactTrieNodeList* list );
