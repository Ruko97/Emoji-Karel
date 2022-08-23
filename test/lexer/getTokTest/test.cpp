#include <cassert>
#include <iostream>
#include "lexer.hpp"

int main(int argc, char **argv) {
	int expectedOutput[] = { Token::tok_while, '(', Token::tok_number, ')', 
   			'{', Token::tok_move, '}', Token::tok_eof };

	int outputLen = sizeof(expectedOutput) / sizeof(int);

	int currentToken;

	for (int i = 0; i < outputLen; i++) {
		currentToken = getTok();
		assert(currentToken == expectedOutput[i]); 
	}

	printf("ALL OK\n");
}
