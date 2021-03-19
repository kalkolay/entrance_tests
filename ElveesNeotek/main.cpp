#include "executor.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cerr << "No valid count arguments" << std::endl;
        return EXIT_FAILURE;
    }

    std::filesystem::path inputDir = argv[1];

    std::string outFile = argv[2];

    if ( !std::filesystem::exists(inputDir) )
    {
        std::cerr << inputDir.string() << " does not exist" << std::endl;
        return EXIT_FAILURE;
    }

    Executor exec(std::move(inputDir), outFile);

    return exec.run();
}
