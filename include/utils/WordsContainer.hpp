#include <string>
#include <map>

/*
  This class is a simple word container that loads the
  entire list of words into memory before allowing access
*/
class WordsContainer
{

  public:

    WordsContainer();
    explicit WordsContainer( const std::string& path_to_words );

    void load_path( const std::string& path_to_words );

    std::map<std::string, unsigned int> word_frequency_map_;

  private:


};
