#include <string>
#include <map>
#include <iostream>
#include <fstream>

class BasicTrie
{
  public:

    BasicTrie( void );
    explicit BasicTrie( const std::string& word, unsigned int frequency );

    void add_word( const std::string& word, unsigned int frequency );

    void print( std::ostream& out );
    void print_words( std::ostream& out );
    void print_words_frequencies( std::ostream& out );

    void serialise( const std::string& filename );
    void load( const std::string& filename );

    unsigned int count_nodes( void );

  private:

    void print_aux( std::ostream& out , std::string buf );
    void print_words_aux( std::ostream& out, std::string buf );
    void print_words_frequencies_aux( std::ostream& out, std::string buf );

    void set_address( unsigned int address );
    unsigned int set_address_aux( unsigned int address );

    void serialise_aux( std::ofstream& file, char value );

    std::map< char, BasicTrie* > children_;

    unsigned int address_;
    unsigned int frequency_;

};
