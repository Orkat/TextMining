#include "utils/CommandLineArguments.hpp"

#include <stdlib.h>


AppCommandLineArguments::AppCommandLineArguments( int argc, char** argv )
{
  print_words_ = false;
  if ( argc >= 2 )
  {
    auto arg1 = std::string( argv[ 1 ] );

    if ( arg1 == "-h" || arg1 == "--help" )
    {
      print_usage( std::cout );
      exit( 0 );
    }
    else
    {
      path_to_dict_ = arg1;
    }

    if ( argc >= 3 )
    {
      auto arg2 = std::string( argv[ 2 ] );
      if ( arg2 == "--print_words")
        print_words_ = true;
      else
      {
        print_usage( std::cerr );
        exit( 1 );
      }
    }

  }
  else
  {
    print_usage( std::cerr );
    exit( 1 );
  }
}

void AppCommandLineArguments::print_usage( std::ostream& out )
{
  out << "./TextMiningApp /path/to/dict.bin\n";
  out << std::flush;
}


CompilerCommandLineArguments::CompilerCommandLineArguments( int argc, char** argv )
{
  print_words_ = false;
  if ( argc >= 3 )
  {
    auto arg1 = std::string( argv[ 1 ] );
    auto arg2 = std::string( argv[ 2 ] );

    if ( arg1 == "-h" || arg1 == "--help" || arg2 == "-h" || arg2 == "--help" )
    {
      print_usage( std::cout );
      exit( 0 );
    }
    else
    {
      path_to_words_ = arg1;
      path_to_dict_ = arg2;
    }

    if ( argc >= 4 )
    {
      auto arg3 = std::string( argv[ 3 ] );
      if ( arg3 == "--print_words")
        print_words_ = true;
      else
      {
        print_usage( std::cerr );
        exit( 1 );
      }
    }

  }
  else
  {
    print_usage( std::cerr );
    exit( 1 );
  }
}

void CompilerCommandLineArguments::print_usage( std::ostream& out )
{
  out << "./TextMiningCompiler /path/to/words.txt /path/to/dict.bin\n";
  out << std::flush;
}
