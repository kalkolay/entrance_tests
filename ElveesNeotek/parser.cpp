#include "parser.h"

#include <fstream>

#include <boost/algorithm/string.hpp>

Parser::Parser(std::filesystem::path filename) 
    : _filename( std::move(filename) )
{

}

std::string Parser::parse()
{
    std::ifstream in(_filename);

    std::string text;
    std::getline(in, text);

    std::vector<std::string> patterns;
    std::string pattern;
    while ( std::getline(in, pattern) )
        patterns.push_back(pattern);

    for (auto& pattern : patterns)
    {
        std::vector<boost::iterator_range<std::string::const_iterator>> matches;
        boost::find_all(matches, text, pattern);
        for (auto match : matches)
        {
            auto pos = match.begin() - text.begin();
            text.replace(text.begin() + pos, text.begin() + pos + pattern.length(), "\n");
        }
    }

    text = "\"" + _filename.filename().string() + "\"" + "\n" + text + "\n\n";
    return text;
}
