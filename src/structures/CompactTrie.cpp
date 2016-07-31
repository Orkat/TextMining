#include "structures/CompactTrie.hpp"
#include "utils/Functions.hpp"
#include "utils/Defines.hpp"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>

CompactTrie::CompactTrie()
{
  root_node_ = nullptr;
}

void CompactTrie::add_word( const std::string& word, unsigned int frequency )
{
  //std::cout << word << std::endl;
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
    current_offset = serialise_aux( file, children->node_, current_offset );

    file.seekp( children_offset + COMPACT_TRIE_VALUE_SIZE + COMPACT_TRIE_FREQUENCY_SIZE, std::ios::beg );
    write_binary_unsigned_int( file, current_offset, COMPACT_TRIE_CHILDREN_OFFSET_SIZE );
    file.seekp( current_offset, std::ios::beg );

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

    file.write( &node->value_, COMPACT_TRIE_VALUE_SIZE );
    write_binary_unsigned_int( file, node->frequency_, COMPACT_TRIE_FREQUENCY_SIZE );

    write_binary_unsigned_int( file, 0, COMPACT_TRIE_CHILDREN_OFFSET_SIZE );

    write_binary_unsigned_int( file, calculate_list_size( root_node_->children_ ), COMPACT_TRIE_N_CHILDREN_SIZE );

    children = children->next_;
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
    return insert_node_lower_bound( parent->children_, value );
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
