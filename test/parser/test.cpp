#include <iostream>
#include "util.hpp"
#include "parser.hpp"

static void MainLoop() {
	while (true) {
		switch (CurTok) {
		case tok_eof:
			return;
		default:
			ParseProgram();
		}
	}
}

int main(int argc, char **argv) {
	SetupBinops();
	
	MainLoop();
	
	std::cout << "ALL OK" << std::endl;
}
