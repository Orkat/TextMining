#include <string>
#include <utility>
#include <fstream>

/*
  This is an improved word container that allows
  the user to iterate over the list of words in
  a file without loading the entire file into
  memory
*/
class WordsIterator
{

  public:

    WordsIterator();
    explicit WordsIterator( const std::string& path_to_words );

    void load_path( const std::string& path_to_words );
    std::pair<std::string, unsigned int>* get_next();

  private:

    std::ifstream words_file_;

};
