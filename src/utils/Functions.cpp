#include "utils/Functions.hpp"

#include <iomanip>

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
