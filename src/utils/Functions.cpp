#include "utils/Functions.hpp"

#include <iomanip>
#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <iostream>

unsigned int next_power_of_2( const unsigned int val )
{
  unsigned int k = 1;
  unsigned int p = 0;

  while ( k < val )
  {
    k *= 2;
    p += 1;
  }

  return p;
}

void set_unsigned_int_buffer( const unsigned int value, char* buffer, unsigned int n_bytes )
{
  unsigned int tmp = value;
  for ( unsigned int i = 0; i < n_bytes; ++i )
  {
    unsigned int buffer_u = tmp % 256;
    buffer[i] = buffer_u - 128;
    tmp /= 256;
  }
}

unsigned int get_unsigned_int_buffer( char* buffer, unsigned int n_bytes )
{
  unsigned int tmp = 1;
  unsigned int ret = 0;
  for ( unsigned int i = 0; i < n_bytes; ++i )
  {
    unsigned char buffer_u = buffer[i] + 128;
    ret += buffer_u * tmp;
    tmp *= 256;
  }
  return ret;
}

void write_binary_unsigned_int( std::ofstream& file, unsigned int value, unsigned int n_bytes )
{
  char* buffer = new char[ n_bytes ];
  set_unsigned_int_buffer( value, buffer, n_bytes );
  file.write( buffer, n_bytes );
  delete buffer;
}

unsigned int read_binary_unsigned_int( std::ifstream& file, unsigned int n_bytes )
{
  char* buffer = new char[ n_bytes ];
  file.read( buffer, n_bytes );
  unsigned int ret = get_unsigned_int_buffer(buffer, n_bytes);
  delete buffer;
  return ret;
}

unsigned int read_binary_unsigned_int_void_ptr( void* ptr, unsigned int n_bytes )
{
  char* buffer = new char[ n_bytes ];
  for ( unsigned int i = 0; i < n_bytes; ++i )
    buffer[i] = static_cast<char*>(ptr)[i];
  unsigned int ret = get_unsigned_int_buffer(buffer, n_bytes);
  delete buffer;
  return ret;
}

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void* offset_void_pointer( void* ptr, unsigned int offset )
{
  char* char_ptr = static_cast<char*>(ptr);
  char_ptr += offset;
  return static_cast<void*>(char_ptr);
}

void print_file_hex(const char* filename, unsigned int n_bytes)
{
  std::ifstream file(filename, std::ifstream::binary);

  int i = 0;
  while ( i < n_bytes )
  {
    unsigned int c = read_binary_unsigned_int(file, 1);
    //std::cout << std::dec << "c : " << (int)c_u << std::endl;
    //std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)c_u;
    std::cout << c;
    if ( i % 8 < 7 )
      std::cout << " ";
    else
      std::cout << std::endl;
    i += 1;
  }
  std::cout << std::endl;
}

unsigned int damerau_levenshtein_distance( const char* str1, const char* str2 )
{
  unsigned int len1 = std::strlen(str1);
  unsigned int len2 = std::strlen(str2);

  unsigned int** d = new unsigned int*[len1+1];
  for ( unsigned int i = 0; i < len1+1; ++i )
    d[i] = new unsigned int[len2+1];

  int cost = 0;

  for ( unsigned int i = 0; i < len1+1; ++i )
    d[i][0] = i;
  for ( unsigned int j = 0; j < len2+1; ++j )
    d[0][j] = j;

  for ( unsigned int i = 1; i < len1+1; ++i )
  {
    for ( unsigned int j = 1; j < len2+1; ++j )
    {
      if ( str1[i-1] == str2[j-1] )
        cost = 0;
      else
        cost = 1;

      d[i][j] = std::min( d[i-1][j] + 1, std::min(d[i][j-1] + 1, d[i-1][j-1] + cost ));

      if ( i > 1 && j > 1 && str1[i-1] == str2[j-2] && str1[i-2] == str2[j-1] )
        d[i][j] = std::min( d[i][j], d[i-2][j-2] + 1 );
    }
  }

  unsigned int value = d[len1][len2];
  for ( unsigned int i = 0 ; i < len1 + 1; ++i )
    delete d[i];

  delete d;

  return value;
}

void sort_dl_words( std::vector<std::tuple<std::string, unsigned int, unsigned int> >& dl_words )
{
  std::sort( dl_words.begin(), dl_words.end(), less_than_dl_word() );
}
