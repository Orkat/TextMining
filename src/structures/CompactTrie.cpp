#include "structures/CompactTrie.hpp"

#include <stdio.h>
#include <string.h>
#include <iostream>

CompactTrie::CompactTrie()
{
  root_node_ = nullptr;
}

void CompactTrie::add_word( const std::string& word, unsigned int frequency )
{
  std::cout << word << std::endl;
  if ( root_node_ == nullptr )
  {
    root_node_ = new CompactTrieNode();
    root_node_->children_ = nullptr;
  }

  add_word_aux( root_node_, word.c_str(), frequency );
}

void CompactTrie::add_word_aux( CompactTrieNode* compact_trie_node, const char* word, unsigned int frequency )
{
  if ( strlen( word ) == 0 )
    compact_trie_node->frequency_ = frequency;
  else
  {
    CompactTrieNode* child = add_child_node( compact_trie_node, word[0] );
    add_word_aux( child, &word[1], frequency );
  }
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
