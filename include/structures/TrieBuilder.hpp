#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

class Trie;

class TrieBuilder
{
  typedef std::map<std::string, TrieBuilder*> TrieLayer;
  typedef std::map<std::string, unsigned int> IndexStrMap;

public:

  TrieBuilder(void);
  TrieBuilder(unsigned int freq);

  void add_word(std::string word, unsigned int freq);
  void print_words(std::string str);
  void serialize(const std::string& outputPath);
  void* write_trie(IndexStrMap strmap, void* trieBuff);
  void compress(std::string key, TrieBuilder* builder, bool stack);
  std::pair<int, int> size();
  void write_string_index(IndexStrMap strmap, void* strIndexBuff);
  IndexStrMap build_string_index( IndexStrMap* strmap, unsigned int* index);

private:
  TrieLayer children_;
  unsigned int frequency_;
};

/*
 *  Serialized Node
 */
class TrieNode
{
public:
  TrieNode(unsigned int index, unsigned int freq);

  unsigned int get_string_index();
  bool is_terminal();
  unsigned int get_freq();
  unsigned int get_offset();
  void set_offset(unsigned int offset);

private:
  unsigned int str_index_;
  unsigned int freq_;
  unsigned int offset_;
};


/**
 *  Serialized Layer pointing towards all children
 */
class mNode
{
public:
  inline mNode* get_child(const std::string, Trie* trie);

  unsigned int count;
  TrieNode trieNd[0];
};

/*
 *  Read Only Serialized Object
 */
class Trie
{
public:
  Trie(char* dictPath);
  void print_words(std::string str);
  void print_child(std::string str, mNode* node);
  std::string get_string(unsigned int index);
  mNode* get_root();

private:
  int str_buff_size_;
  void* string_index_;
  void* data_;
};

