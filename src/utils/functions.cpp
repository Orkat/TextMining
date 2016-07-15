#include "utils/functions.hpp"

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
