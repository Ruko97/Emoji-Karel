#include <iostream>
#include "util.hpp"
#include "parser.hpp"

int main(int argc, char **argv) {
	SetupBinops();
	getNextToken();
	
	auto program = ParseProgram();
	
	std::cout << "ALL OK" << std::endl;
	
	program->dump(0);
}
