#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>

unsigned int next_power_of_2( const unsigned int val );

void set_unsigned_int_buffer( const unsigned int value, char* buffer, unsigned int n_bytes );

unsigned int get_unsigned_int_buffer( char* buffer, unsigned int n_bytes );

void write_binary_unsigned_int( std::ofstream& file, unsigned int value, unsigned int n_bytes );

unsigned int read_binary_unsigned_int( std::ifstream& file, unsigned int n_bytes );

unsigned int read_binary_unsigned_int_void_ptr( void* ptr, unsigned int n_bytes );

std::ifstream::pos_type filesize(const char* filename);

void* offset_void_pointer( void* ptr, unsigned int offset );

void print_file_hex(const char* filename, unsigned int n_bytes);

unsigned int damerau_levenshtein_distance( const char* str1, const char* str2 );

void sort_dl_words( std::vector<std::tuple<std::string, unsigned int, unsigned int> >& dl_words );

struct less_than_dl_word
{
  inline bool operator() ( const std::tuple<std::string, unsigned int, unsigned int>& lhs, const std::tuple<std::string, unsigned int, unsigned int>& rhs )
  {
    if ( std::get<1>( lhs ) < std::get<1>( rhs ) )
      return true;
    else if ( std::get<1>( lhs ) == std::get<1>( rhs ) )
    {
      if ( std::get<2>( lhs ) < std::get<2>( rhs ) )
        return true;
      else if ( std::get<2>( lhs ) == std::get<2>( rhs ) )
      {
        std::string lhs_str = std::get<0>( lhs );
        std::string rhs_str = std::get<0>( rhs );
        if ( lhs_str.compare( rhs_str ) < 0 )
          return true;
      }
    }

    return false;
  }
};
