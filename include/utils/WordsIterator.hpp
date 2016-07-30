#include <string>
#include <utility>
#include <fstream>

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
