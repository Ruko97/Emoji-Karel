// NOTE: WE EXPECT ASSERTION TO FAIL HERE

#include <cassert>
#include <iostream>
#include "../../../lexer.h"

int main(int argc, char **argv) {
	int expectedOutput[] = { Token::tok_while, '(', Token::tok_number, ')', 
   			Token::tok_move, '}', Token::tok_eof };

	int outputLen = sizeof(expectedOutput) / sizeof(int);

	int currentToken;

	for (int i = 0; i < currentToken; i++) {
		currentToken = getTok();
		assert(currentToken == expectedOutput[i]); 
	}

	printf("ALL OK\n");
}
