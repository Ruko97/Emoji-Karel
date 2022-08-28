#include <iostream>
#include "util.hpp"
#include "parser.hpp"

int main(int argc, char **argv) {
	SetupBinops();
	getNextToken();

	auto program = ParseProgram();

	std::cout << "PARSING OK" << std::endl;

	std::cout << std::endl;
	program->codegen(std::cout);
	std::cout << std::endl;

	std::cout << "ALL OK" << std::endl;
	return 0;
}
