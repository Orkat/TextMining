#include "structures/CompactTrie.hpp"
#include "utils/Functions.hpp"
#include "utils/Defines.hpp"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <cmath>

CompactTrie::CompactTrie()
{
  root_node_ = nullptr;
}

void CompactTrie::add_word( const std::string& word, unsigned int frequency )
{
  if ( root_node_ == nullptr )
  {
    root_node_ = new CompactTrieNode();
    root_node_->children_ = nullptr;
  }

  add_word_aux( root_node_, word.c_str(), frequency );
}

void CompactTrie::add_word_aux( CompactTrieNode* node, const char* word, unsigned int frequency )
{
  if ( strlen( word ) == 0 )
    node->frequency_ = frequency;
  else
  {
    CompactTrieNode* child = add_child_node( node, word[0] );
    add_word_aux( child, &word[1], frequency );
  }
}

void CompactTrie::serialise( const std::string& filename )
{
  std::ofstream file( filename , std::ios::out | std::ios::binary );
  write_binary_unsigned_int( file, calculate_list_size( root_node_->children_ ), COMPACT_TRIE_N_CHILDREN_SIZE );
  serialise_aux( file, root_node_, COMPACT_TRIE_N_CHILDREN_SIZE );

  file.close();
}

unsigned int CompactTrie::serialise_aux( std::ofstream& file, const CompactTrieNode* node, unsigned int current_offset )
{
  serialise_children_list( file, node->children_ );

  unsigned int children_offset = current_offset;
  current_offset += calculate_list_size( node->children_ ) * COMPACT_TRIE_NODE_SIZE;

  CompactTrieNodeList* children = node->children_;
  while ( children != nullptr )
  {
    file.seekp( children_offset + COMPACT_TRIE_VALUE_SIZE + COMPACT_TRIE_FREQUENCY_SIZE, std::ios::beg );
    write_binary_unsigned_int( file, current_offset, COMPACT_TRIE_CHILDREN_OFFSET_SIZE );
    file.seekp( current_offset, std::ios::beg );

    current_offset = serialise_aux( file, children->node_, current_offset );

    children_offset += COMPACT_TRIE_NODE_SIZE;
    children = children->next_;
  }

  return current_offset;
}

void CompactTrie::serialise_children_list( std::ofstream& file, const CompactTrieNodeList* children )
{
  //std::cout << "serialise_children_list , n_children : " << calculate_list_size( root_node_->children_ ) << std::endl;
  while( children != nullptr )
  {
    CompactTrieNode* node = children->node_;

    write_binary_unsigned_int( file, node->value_, COMPACT_TRIE_VALUE_SIZE );
    write_binary_unsigned_int( file, node->frequency_, COMPACT_TRIE_FREQUENCY_SIZE );
    write_binary_unsigned_int( file, 0, COMPACT_TRIE_CHILDREN_OFFSET_SIZE );
    write_binary_unsigned_int( file, calculate_list_size( node->children_ ), COMPACT_TRIE_N_CHILDREN_SIZE );

    children = children->next_;
  }
}

std::vector<std::tuple<std::string, unsigned int, unsigned int> > CompactTrie::get_dlwords(std::string word, unsigned int distance)
{
  std::vector<std::tuple<std::string, unsigned int, unsigned int> > ret;
  unsigned int n_children = read_binary_unsigned_int_void_ptr( file_mmap_, COMPACT_TRIE_N_CHILDREN_SIZE );
  unsigned int offset = COMPACT_TRIE_N_CHILDREN_SIZE;
  for ( unsigned int i = 0; i < n_children; ++i )
  {
    auto vect = get_dlwords_aux( offset, std::string(), word, distance, std::string() + word[0] );
    ret.insert( ret.end(), vect.begin(), vect.end() );
    offset += COMPACT_TRIE_NODE_SIZE;
  }
  return ret;
}

std::vector<std::tuple<std::string, unsigned int, unsigned int> > CompactTrie::get_dlwords_aux( unsigned int offset, std::string current_word, std::string word, unsigned int distance, std::string word_partial )
{
  unsigned int value = read_binary_unsigned_int_void_ptr( offset_void_pointer(file_mmap_, offset), COMPACT_TRIE_VALUE_SIZE );
  offset += COMPACT_TRIE_VALUE_SIZE;
  unsigned int frequency = read_binary_unsigned_int_void_ptr( offset_void_pointer(file_mmap_, offset), COMPACT_TRIE_FREQUENCY_SIZE );
  offset += COMPACT_TRIE_FREQUENCY_SIZE;
  unsigned int children_offset = read_binary_unsigned_int_void_ptr( offset_void_pointer(file_mmap_, offset), COMPACT_TRIE_CHILDREN_OFFSET_SIZE );
  offset += COMPACT_TRIE_CHILDREN_OFFSET_SIZE;
  unsigned int n_children = read_binary_unsigned_int_void_ptr( offset_void_pointer(file_mmap_, offset), COMPACT_TRIE_N_CHILDREN_SIZE );
  offset += COMPACT_TRIE_N_CHILDREN_SIZE;

  current_word += (char)value;

  unsigned int current_distance = damerau_levenshtein_distance(current_word.c_str(), word.c_str());
  unsigned int current_distance_partial = damerau_levenshtein_distance(current_word.c_str(), word_partial.c_str());

  std::vector<std::tuple<std::string, unsigned int, unsigned int> > ret;

  if ( distance == 0 && current_distance_partial > 0 )
    return ret;

  unsigned int length_diff = std::abs( (int)current_word.size() - (int)word.size() );

  if ( current_distance_partial > distance*2 )
    return ret;


  if ( n_children == 0 || frequency != 0 )
  {
    if ( current_distance <= distance )
      ret.push_back( std::make_tuple( current_word, current_distance, frequency ) );
  }

  if ( n_children > 0 )
  {
    for ( unsigned int i = 0; i < n_children; ++i )
    {
      std::string new_word_partial = word_partial;
      if ( word_partial.size() < word.size() )
        new_word_partial += word[current_word.size()];
      auto vect = get_dlwords_aux( children_offset + i*COMPACT_TRIE_NODE_SIZE, current_word, word, distance, new_word_partial );
      ret.insert(ret.end(), vect.begin(), vect.end());
    }
  }

  return ret;
}

void CompactTrie::load_mmap( const std::string& filename )
{
  unsigned int file_size = filesize( filename.c_str() );
  int fd = open( filename.c_str(), O_RDONLY );
  file_mmap_ = mmap(0, file_size, PROT_READ, MAP_PRIVATE, fd, 0 );
}

void CompactTrie::print_words( void )
{
  print_words_aux( root_node_, std::string() );
}

void CompactTrie::print_words_aux( const CompactTrieNode* node, std::string word )
{
  if ( node->children_ == nullptr || node->frequency_ != 0 )
    std::cout << word << std::endl;

  CompactTrieNodeList* children = node->children_;
  while( children != nullptr )
  {
    print_words_aux( children->node_ , word + children->node_->value_ );
    children = children->next_;
  }
}

void CompactTrie::print_words_mmap( void )
{
  unsigned int n_children = read_binary_unsigned_int_void_ptr( file_mmap_, COMPACT_TRIE_N_CHILDREN_SIZE );
  unsigned int offset = COMPACT_TRIE_N_CHILDREN_SIZE;
  for ( unsigned int i = 0; i < n_children; ++i )
  {
    print_words_mmap_aux( offset, std::string() );
    offset += COMPACT_TRIE_NODE_SIZE;
  }
}

void CompactTrie::print_words_mmap_aux( unsigned int offset, std::string word )
{
  //std::cout << "print_words_mmap_aux , offset : " << offset << " , word : " << word << std::endl;

  unsigned int value = read_binary_unsigned_int_void_ptr( offset_void_pointer(file_mmap_, offset), COMPACT_TRIE_VALUE_SIZE );
  offset += COMPACT_TRIE_VALUE_SIZE;
  unsigned int frequency = read_binary_unsigned_int_void_ptr( offset_void_pointer(file_mmap_, offset), COMPACT_TRIE_FREQUENCY_SIZE );
  offset += COMPACT_TRIE_FREQUENCY_SIZE;
  unsigned int children_offset = read_binary_unsigned_int_void_ptr( offset_void_pointer(file_mmap_, offset), COMPACT_TRIE_CHILDREN_OFFSET_SIZE );
  offset += COMPACT_TRIE_CHILDREN_OFFSET_SIZE;
  unsigned int n_children = read_binary_unsigned_int_void_ptr( offset_void_pointer(file_mmap_, offset), COMPACT_TRIE_N_CHILDREN_SIZE );
  offset += COMPACT_TRIE_N_CHILDREN_SIZE;

  //std::cout << "value : " << value << " , frequency : " << frequency << " , children_offset : " << children_offset << " , n_children : " << n_children << std::endl;

  if ( n_children == 0 || frequency != 0 )
    std::cout << word + (char)value << std::endl;

  if ( n_children > 0 )
  {
    for ( unsigned int i = 0; i < n_children; ++i )
      print_words_mmap_aux( children_offset + i*COMPACT_TRIE_NODE_SIZE, word + (char)value );
  }
}

std::pair<bool, unsigned int> CompactTrie::get_children_offset( const CompactTrieNode* current_node, const CompactTrieNode* node )
{
  //std::cout << "get_children_offset , value : " << current_node->value_ << std::endl;
  if ( current_node == node )
    return std::make_pair( true, 0 );

  unsigned int node_size = COMPACT_TRIE_VALUE_SIZE + COMPACT_TRIE_FREQUENCY_SIZE + COMPACT_TRIE_CHILDREN_OFFSET_SIZE + COMPACT_TRIE_N_CHILDREN_SIZE;
  unsigned int offset = calculate_list_size( node->children_ ) * node_size;

  CompactTrieNodeList* children = current_node->children_;

  while ( children != nullptr )
  {
    CompactTrieNode* local_node = children->node_;
    //std::cout << "offset : " << offset << std::endl;
    std::pair<bool, unsigned int> local_offset = get_children_offset( local_node, node );
    if ( local_offset.first )
      return std::make_pair( true, offset + local_offset.second );

    offset += local_offset.second;

    children = children->next_;
  }

  return std::make_pair( false, offset );
}

CompactTrieNode* add_child_node( CompactTrieNode* parent, char value )
{
  if ( parent->children_ == nullptr )
  {
    parent->children_ = new CompactTrieNodeList();
    parent->children_->next_ = nullptr;
    parent->children_->node_ = nullptr;
  }

  CompactTrieNode* child = find_node( parent->children_, value );
  if ( child == nullptr )
    return append_node( parent->children_, value );
  return child;
}

CompactTrieNode* find_node( CompactTrieNodeList* list, char value )
{
  while ( list != nullptr )
  {
    if ( list->node_ == nullptr )
      return nullptr; // head of list has not yet been initialised
    if ( list->node_->value_ == value )
      return list->node_;
    list = list->next_;
  }
  return nullptr;
}

CompactTrieNode* append_node( CompactTrieNodeList* list, char value )
{
  if ( list->node_ == nullptr )
  {
    CompactTrieNode* node = new CompactTrieNode();
    node->value_ = value;
    node->frequency_ = 0;
    list->node_ = node;

    return node;
  }
  else
  {
    while ( list->next_ != nullptr )
      list = list->next_;

    CompactTrieNodeList* new_list = new CompactTrieNodeList();
    new_list->next_ = nullptr;
    list->next_ = new_list;

    CompactTrieNode* node = new CompactTrieNode();
    node->value_ = value;

    new_list->node_ = node;

    return node;
  }
}

CompactTrieNode* insert_node_lower_bound( CompactTrieNodeList* list, char value )
{
  if ( list->node_ == nullptr )
  {
    CompactTrieNode* node = new CompactTrieNode();
    node->value_ = value;
    node->frequency_ = 0;
    list->node_ = node;

    return node;
  }
  else
  {

      while ( list->next_ != nullptr && list->next_->node_->value_ < value )
        list = list->next_;

      CompactTrieNodeList* new_list = new CompactTrieNodeList();
      new_list->next_ = list->next_;
      list->next_ = new_list;

      CompactTrieNode* node = new CompactTrieNode();
      node->value_ = value;

      new_list->node_ = node;

      return node;
    }

}

unsigned int calculate_list_size( CompactTrieNodeList* list )
{
  unsigned int ret = 0;
  while ( list != nullptr )
  {
    list = list->next_;
    ret += 1;
  }
  return ret;
}
