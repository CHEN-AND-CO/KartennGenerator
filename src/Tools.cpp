#include "Tools.hpp"

// Split a string into an array of substring using one or several delimiter chars
std::vector<std::string> split(std::string str, std::string delim) {
    std::vector<std::string> tokens;
    std::string::size_type last_token_start = 0, i = 0;

    for (i = 0; i < str.size(); ++i)
    {
        if ( delim.find(str[i]) != std::string::npos ) {
            tokens.push_back(str.substr(last_token_start, i - last_token_start));
            last_token_start = i+1;
        }
    }
    tokens.push_back(str.substr(last_token_start, i - last_token_start));

    return tokens;
}