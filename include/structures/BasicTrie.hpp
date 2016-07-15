#include <string>
#include <map>
#include <iostream>
#include <fstream>

class BasicTrie
{
  public:

    BasicTrie( void );
    explicit BasicTrie( const std::string& word );

    void add_word( const std::string& word );
    void print( std::ostream& out );
    void serialise( const std::string& filename );
    unsigned int count_nodes( void );

  private:

    void print_aux( std::ostream& out , std::string buf );

    void set_address( unsigned int address, unsigned int n_bits_per_node_value, unsigned int n_bits_per_address );
    unsigned int set_address_aux( unsigned int address, unsigned int n_bits_per_node_value, unsigned int n_bits_per_address );

    void serialise_aux( std::ofstream& file, char value, unsigned int n_bytes_per_value, unsigned int n_bytes_per_address );

    std::map< char, BasicTrie* > children_;
    unsigned int address_;

};
