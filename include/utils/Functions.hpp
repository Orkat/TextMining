#include <iostream>
#include <fstream>

unsigned int next_power_of_2( const unsigned int val );

void set_unsigned_int_buffer( const unsigned int value, char* buffer, unsigned int n_bytes );

unsigned int get_unsigned_int_buffer( char* buffer, unsigned int n_bytes );

void write_binary_unsigned_int( std::ofstream& file, unsigned int value, unsigned int n_bytes );

unsigned int read_binary_unsigned_int( std::ifstream& file, unsigned int n_bytes );

unsigned int read_binary_unsigned_int_void_ptr( void* ptr, unsigned int n_bytes );

std::ifstream::pos_type filesize(const char* filename);

void* offset_void_pointer( void* ptr, unsigned int offset );

void print_file_hex(const char* filename, unsigned int n_bytes);
