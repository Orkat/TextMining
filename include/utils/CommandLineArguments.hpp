#include <string>
#include <iostream>

class AppCommandLineArguments
{
  public:

    AppCommandLineArguments( int argc, char** argv );

    std::string path_to_dict_;
    bool print_words_;

  private:

    void print_usage( std::ostream& out );

};

class CompilerCommandLineArguments
{
  public:

    CompilerCommandLineArguments( int argc, char** argv );

    std::string path_to_words_;
    std::string path_to_dict_;
    bool print_words_;

  private:

    void print_usage( std::ostream& out );

};
