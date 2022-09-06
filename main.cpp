#include <SFML/Window.hpp>

#include <fstream>

#include "util.hpp"
#include "parser.hpp"
#include "world.hpp"

int main(int argc, char **argv) {
    SetupBinops();
    getNextToken();

    auto program = ParseProgram();

    std::ofstream ofs(argc >= 2 ? argv[1] : "temp.krl", std::ios::out);

    program->codegen(ofs);

    return 0;
}
