//
// Created by Rafael Campos on 3/26/16.
//

#ifndef NIKITA_RENDER_SMFREADER_H
#define NIKITA_RENDER_SMFREADER_H

#include "../core/nikita.h"
#include "stringUtils.h"
#include <fstream>

namespace nikita
{
class SmfReader
{
public:
    typedef std::vector<float> Vertices;
    typedef std::vector<int> Indices;

    void static getGeometry(const char* filename, Vertices& positions, Indices& indices)
    {
        buildMesh(readFile(filename), positions, indices);
    }

private:
    std::string static readFile(const char* filename)
    {
        std::string fileContent;
        std::ifstream objFile(filename, std::ios::in);

        if (!objFile.good())
        {
            std::cout << "Error trying to read " << filename << std::endl;
            std::terminate();
        }

        objFile.seekg(0, std::ios::end);
        fileContent.resize((unsigned int) objFile.tellg());
        objFile.seekg(0, std::ios::beg);
        objFile.read(&fileContent[0], fileContent.size());
        objFile.close();

        return fileContent;
    }

    bool static isVertex(std::string s)
    {
        return (s == "v");
    }

    void static buildMesh(std::string smf, Vertices& v, Indices& idx)
    {
        v.clear();
        idx.clear();

        std::vector<std::string> tokens = StringUtils::tokenize(smf, " \n");

        for (int i = 0; i < tokens.size(); i+=4)
        {
            if (isVertex(tokens[i]))
            {
                v.push_back(std::stof(tokens[i+1]));
                v.push_back(std::stof(tokens[i+2]));
                v.push_back(std::stof(tokens[i+3]));
            }
            else
            {
                idx.push_back(std::stoi(tokens[i+1]) - 1);
                idx.push_back(std::stoi(tokens[i+2]) - 1);
                idx.push_back(std::stoi(tokens[i+3]) - 1);
            }
        }
    }
};
}


#endif //NIKITA_RENDER_SMFREADER_H
