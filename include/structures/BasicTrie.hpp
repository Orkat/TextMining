#include <string>
#include <map>
#include <iostream>

class BasicTrie
{
  public:

    BasicTrie( void );
    explicit BasicTrie( const std::string& word );

    void add_word( const std::string& word );
    void print( std::ostream& out );

  private:

    void print_aux( std::ostream& out , std::string buf );

    std::map< char, BasicTrie* > children_;

};
