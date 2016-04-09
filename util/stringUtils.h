//
// Created by Rafael Campos on 3/27/16.
//

#ifndef NIKITA_RENDER_STRINGUTILS_H
#define NIKITA_RENDER_STRINGUTILS_H

#include <iostream>
#include <string>
#include <vector>

namespace nikita
{
class StringUtils
{
public:
    void static toLower(std::basic_string<char>& s);

    std::vector<std::string> static tokenize(const std::string &s, const char* delimiter);

};
}

#endif //NIKITA_RENDER_STRINGUTILS_H
