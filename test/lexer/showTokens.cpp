#include <iostream>
#include "util.hpp"
#include "lexer.hpp"

int main(int argc, char **argv) {
	int CurTok;
	do {
		CurTok = getTok();
		printToken(CurTok, numVal);
	} while (CurTok != EOF);
	return 0;
}
