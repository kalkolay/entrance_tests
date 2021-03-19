#pragma once

#include <filesystem>

class Parser
{
public:
    explicit Parser(std::filesystem::path filename);

    std::string parse();

private:
    std::filesystem::path _filename;
};
