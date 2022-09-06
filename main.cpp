#include <SFML/Window.hpp>

#include <fstream>

#include "util.hpp"
#include "parser.hpp"
#include "world.hpp"

int main(int argc, char **argv) {
    SetupBinops();
    getNextToken();

    auto program = ParseProgram();

    const char *filename = argc >= 2 ? argv[1] : "temp.krl";

    std::ofstream ofs(filename, std::ios::out);
    if (!ofs)
        std::cerr << "Couldn't create temporary file" << std::endl;

    program->codegen(ofs);

    std::string instruction;
    std::vector<std::string> instructions;

    std::ifstream ifs(filename, std::ios::in);
    if (!ifs)
        std::cerr << "File named " << filename << "not found after adding "
                  << "the genrated program to it" << std::endl;

    while (!ifs.eof()) {
        std::getline(ifs, instruction);
        if (!(ifs.eof() && instruction.length() == 0))
            instructions.push_back(instruction);
    }

    return 0;
}
