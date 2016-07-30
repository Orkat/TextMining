#include "utils/Functions.hpp"

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
    buffer[i] = tmp % 256;
    tmp /= 256;
  }
}

unsigned int get_unsigned_int_buffer( char* buffer, unsigned int n_bytes )
{
  unsigned int tmp = 1;
  unsigned int ret = 0;
  for ( unsigned int i = 0; i < n_bytes; ++i )
  {
    ret += buffer[i] * tmp;
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
