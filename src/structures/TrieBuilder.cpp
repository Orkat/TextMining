#include "structures/TrieBuilder.hpp"
#include "utils/Functions.hpp"
#include "utils/Defines.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

TrieBuilder::TrieBuilder()
{
  frequency_ = 0;
}

TrieBuilder::TrieBuilder( unsigned int freq )
{
  frequency_ = freq;
}

void TrieBuilder::addWord( std::string word, unsigned int freq )
{
  std::map< std::string, TrieBuilder* >::iterator it = children_.find(word.substr(0, 1));
  if (it == children_.end())
  {
    if (word.size() == 1)
      children_.insert(std::make_pair(word.substr(0, 1), new TrieBuilder(freq)));
    else
    {
      TrieBuilder *child = new TrieBuilder();
      children_.insert(std::make_pair(word.substr(0, 1), child));
      child->addWord(word.substr(1, word.size() - 1), freq);
    }
  }
  else
  {
    it->second->addWord(word.substr(1, word.size() - 1), freq);
  }
}

void TrieBuilder::printWords( std::string str )
{
  if (frequency_ != 0)
    std::cout << str << " " << frequency_ << std::endl;
  for (auto const &entry : children_)
  {
    std::stringstream ss;
    ss << str << entry.first;
    entry.second->printWords(ss.str());
  }
}


void* TrieBuilder::writeTrie( std::map<std::string, unsigned int> strmap, void* trieBuff) {

  if (children_.size() == 0)
    return trieBuff;
  void *save = trieBuff;
  mNode* node = static_cast<mNode*>(trieBuff);
  node->count = children_.size();

  int i = 0;
  for (auto child : children_) {
    TrieNode *nd = new(&node->trieNd[i]) TrieNode(strmap[child.first], child.second->frequency_);
    ++i;
  }

  // displace by offset and recursive call to construct object buffer
  void* endpointer = &node->trieNd[i];
  void* oldpointer = endpointer;

  unsigned int offset = 0;
  int index = 0;
  for (auto child : children_)
  {
    offset = static_cast<char*>(endpointer) - reinterpret_cast<char*>(&node->trieNd[index]);
    node->trieNd[index].setOffset(offset);
    endpointer = child.second->writeTrie(strmap, endpointer);
    if (child.second->children_.size() == 0 || endpointer == oldpointer )
      node->trieNd[index].setOffset(0);
    index++;
  }

  return endpointer;
}

std::pair<int, int> TrieBuilder::size()
{
  if (children_.size() == 0)
    return std::make_pair(0, 1);

  std::pair<int, int> totalCount = std::make_pair(children_.size(), 0);
  std::pair<int, int> childCount;

  for (auto child : children_)
  {
    childCount = child.second->size();
    totalCount.first += childCount.first;
    totalCount.second += childCount.second;
  }

  return totalCount;
}

void TrieBuilder::serialize( const std::string& outputPath )
{

  unsigned int* index;
  unsigned int i = 0;
  index = &i;

  std::map<std::string, unsigned int> strmap;
  strmap = buildStringIndex(&strmap, index);


  // String inverted index size
  int strBuffSize = 0;
  for (auto elt : strmap) {
    std::cout << elt.first << std::endl;
    strBuffSize += elt.first.size() + 1;
  }

  // Size in memory of the Trie
  std::pair<int, int> res = size();

  int trieBuffSize = (res.first) * sizeof(TrieNode) + (res.first - res.second + 1) * sizeof(mNode::count);


  // Allocate for object buffer and string index
  void* buff = malloc(trieBuffSize + strBuffSize);
  int* bff = (int *) buff;
  *bff = strBuffSize;
  char* tpointer = (char *) buff + sizeof(int);
  void* strBuff = (void *) tpointer;
  tpointer = (char *) strBuff + strBuffSize;
  void* trieBuff = tpointer;


  // Construct Object Buffer

  writeStringIndex(strmap, strBuff);
  writeTrie(strmap, trieBuff);

  // Write Object Buffer

  std::ofstream output(outputPath, std::ios::out | std::ios::binary);
  output.write(static_cast<char*>(buff), sizeof(int) + strBuffSize + trieBuffSize);
  output.close();
}

void TrieBuilder::writeStringIndex( std::map<std::string, unsigned int> strmap, void* strBuff )
{
  // Write Keys Index

  std::map<unsigned int,std::string> reverseIndex;
  for (auto elt : strmap) {
    reverseIndex[elt.second] = elt.first;
  }


  std::map<unsigned int, std::string>::iterator it;
  int offset = 0;
  char* charbuff = static_cast<char*>(strBuff);

  for (int i = 0; i < reverseIndex.size(); ++i)
  {
    it = reverseIndex.find(offset);

    strcpy(charbuff, it->second.c_str());

    charbuff += it->second.size() + 1;
    offset += it->second.size() + 1;
  }
}



std::map<std::string, unsigned int> TrieBuilder::buildStringIndex( std::map<std::string, unsigned int>* strmap,
                                                                unsigned int* index)
{
  for (auto const child : children_)
  {

    if (strmap->find(child.first) == strmap->end())
    {
      strmap->insert(std::make_pair(child.first, *index));
      *index += child.first.size() + 1;
    }
    child.second->buildStringIndex(strmap, index);
  }

  return *strmap;
}



void TrieBuilder::compress(std::string key, TrieBuilder* parent, bool stack)
{
  if (children_.size() == 0)
    return;

  // remove level and stack it with the next
  if (stack) {

    std::map < std::string, TrieBuilder* >* tmpMap = new std::map < std::string, TrieBuilder* >;
    for (auto child : children_) {
      std::stringstream ss;
      ss << key << child.first;
      tmpMap->insert(std::make_pair(ss.str(), child.second));
    }

    parent->children_.swap(*tmpMap);

  }

  stack = false;

  if (children_.size() == 1) {
    stack = true;
  }

  std::vector<std::string> keys;
  for (auto elt : children_) {
    keys.push_back(elt.first);
  }

  for (auto key: keys)
  {
    children_[key]->compress(key, this, stack);
  }
}


TrieNode::TrieNode( unsigned int index, unsigned int frequency )
{
  strIndex_ = index;
  freq_ = frequency;
}

unsigned int TrieNode::getFreq()
{
  return freq_;
}

unsigned int TrieNode::getStringIndex()
{
  return strIndex_;
}

unsigned int TrieNode::getOffset()
{
  return offset_;
}

void TrieNode::setOffset(unsigned int offset)
{
  offset_ = offset;
}

Trie::Trie( char* dictPath )
{
  char* inputDict = dictPath;
  int fd = open(dictPath, O_RDONLY);
  struct stat fileStat;
  fstat(fd, &fileStat);

  void* raw = mmap(nullptr, fileStat.st_size, PROT_READ, MAP_SHARED, fd, 0);
  strBuffSize_ = *static_cast<int*>(raw);
  char* ptr = (char *) raw + sizeof(int);
  stringIndex_ = (void *) ptr;
  data_ = (void *) (ptr + strBuffSize_);

  mNode* root = (mNode *) data_;
  TrieNode tn = (TrieNode) root->trieNd[1];
}

std::string Trie::getString( unsigned int index )
{
  return (char *) stringIndex_ + index;
}

mNode* Trie::getRoot()
{
  return (mNode *) data_;
}

void Trie::printChild( std::string str, mNode* node )
{

  for (int i = 0; i < node->count; i++) {

    TrieNode tn = (TrieNode) node->trieNd[i];

    std::stringstream ss;
    ss << str << getString(tn.getStringIndex());

    if (tn.getFreq() != 0)
      std::cout << ss.str() << " " << tn.getFreq() << " " << tn.getStringIndex() << std::endl;

    if (tn.getOffset() == 0)
      continue;

    mNode* child = node->getChild(getString(tn.getStringIndex()), this);

    printChild(ss.str(), child);
  }

}

mNode* mNode::getChild( std::string trans , Trie* trie)
{
  int start = 0;
  int end = count - 1;
  int split;
  int pos = -1;
  bool found = false;

  while (!found && start <= end)
  {
    split = (start + end) / 2;
    TrieNode tn = (TrieNode) trieNd[split];
    std::string key = trie->getString(tn.getStringIndex());
    if (key == trans)
    {
      found = true;
      pos = split;
    }
    else if (key > trans)
      end = split - 1;
    else
      start = split + 1;
  }

  TrieNode tn = (TrieNode) trieNd[pos];
  void* child = (char *) &trieNd[pos] + tn.getOffset();
  return (mNode *) child;

}



void Trie::printWords(std::string str)
{
  printChild(str, getRoot());
}

