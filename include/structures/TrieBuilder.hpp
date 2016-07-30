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

    TrieBuilder( void );
    TrieBuilder( unsigned int freq );

    void addWord( std::string word, unsigned int freq );
    void printWords( std::string str );
    void serialize( const std::string& outputPath );
    void* writeTrie( IndexStrMap strmap, void* trieBuff );
    void compress(std::string key, TrieBuilder* builder, bool stack);
    std::pair<int, int> size();
    void writeStringIndex( IndexStrMap strmap, void* strIndexBuff );
    IndexStrMap buildStringIndex( IndexStrMap* strmap, unsigned int* index);
  private:

    TrieLayer children_;
    unsigned int frequency_;

};

/*
 *  Serialized Node
 */
class TrieNode {

public:
    TrieNode( unsigned int index, unsigned int freq );

    unsigned int getStringIndex();
    bool isTerminal();
    unsigned int getFreq();
    unsigned int getOffset();
    void setOffset( unsigned int offset );

private:
    unsigned int strIndex_;
    unsigned int freq_;
    unsigned int offset_;
};


/**
 *  Serialized Layer pointing towards all children
 */
class mNode {

  public:

    inline mNode* getChild( const std::string, Trie* trie );

    unsigned int count;
    TrieNode trieNd[0];
};

/*
 *  Read Only Serialized Object
 */
class Trie {

  public:

    Trie( char* dictPath );
    void printWords( std::string str );
    void printChild( std::string str, mNode* node );
    std::string getString( unsigned int index );
    mNode* getRoot();

  private:

    int strBuffSize_;
    void* stringIndex_;
    void* data_;
};

