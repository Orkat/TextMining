#include <string>
#include <map>

class WordsContainer
{

  public:

    WordsContainer();
    explicit WordsContainer( const std::string& path_to_words );

    void load_path( const std::string& path_to_words );

    std::map<std::string, unsigned int> word_frequency_map_;

  private:




};
