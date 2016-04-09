//
// Created by Rafael Campos on 3/27/16.
//

#include "stringUtils.h"

void nikita::StringUtils::toLower(std::basic_string<char>& s) {
    for (std::basic_string<char>::iterator p = s.begin(); p != s.end(); ++p) {
        *p = std::tolower(*p);
    }
}

std::vector<std::string> nikita::StringUtils::tokenize(const std::string &s, const char* delimiter)
{
    std::size_t beginning = s.find_first_not_of(delimiter);
    std::size_t end;
    std::vector<std::string> result;

    while (beginning != std::string::npos)
    {
        end = s.find_first_of(delimiter, beginning);
        result.push_back(s.substr(beginning, end-beginning));
        beginning = s.find_first_not_of(delimiter, end);
    }
    return result;
}