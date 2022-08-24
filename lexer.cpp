#include <iostream>
#include <cctype>
#include <cassert>
#include "lexer.hpp"

// Returns a 4-byte integer representing the unicode value of the next character
int getNextUnicode() {
	int output = 0;
	int thisChar = getchar();
	
	if (thisChar == EOF) return thisChar;

	if (thisChar >> 7 == 0) return thisChar;

	int remaining_bytes = 0;
	if (thisChar >> 5 == 0x06) {
		output = thisChar & 0x1F;
		remaining_bytes = 1;
	}
	if (thisChar >> 4 == 0x0E) {
		output = thisChar & 0x0F;
		remaining_bytes = 2;
	}
	if (thisChar >> 3 == 0x1E) {
		output = thisChar & 0x07;
		remaining_bytes = 3;
	}

	for (int i = 1; i <= remaining_bytes; i++) {
		output <<= 6;
		thisChar = getchar();
		assert(thisChar != EOF);
		assert(thisChar >> 6 == 0x2);
		output += thisChar & 0x3F;
	}

	return output;
}

int numVal;

int getTok() {
	static int LastChar = ' ';

	// TODO: may add more whitespace characters in this condition in the future
	// We aren't using isspace function here because isspace doesn't support 
	// unicode
	while(LastChar == ' ' || LastChar == '\n' || LastChar == '\t' 
			|| LastChar == '\r' || LastChar == '\v' || LastChar == '\f') {
		LastChar = getNextUnicode();
	}
	
	if (LastChar == EOF)
		return Token::tok_eof;
		
	if (LastChar >= '0' && LastChar <= '9') {
		numVal = 0;
		do {
			numVal = numVal * 10 + (LastChar - '0');
			LastChar = getNextUnicode();
		} while (LastChar >= '0' && LastChar <= '9');
		
		return Token::tok_number;
	}
	
	if (LastChar >= 128) {	// AKA a non-ascii unicode chaacter
		int output = 0;
		
		if (LastChar == 0x27A1) output = Token::tok_move;
		if (LastChar == 0x21A9) output = Token::tok_turn_left;
		if (LastChar == 0x1F914) output = Token::tok_if;
		if (LastChar == 0x1F641) output = Token::tok_else;
		if (LastChar == 0x1F504) output = Token::tok_while;
		if (LastChar == 0x1F6AB) output = Token::tok_not;
		if (LastChar == 0x1F9F1) output = Token::tok_front_blocked;
		
		if (output == 0) fprintf(stderr, "Error: Expected a valid emoji\n");
		
		LastChar = getNextUnicode();
		
		return output;
	}
	
	if (LastChar == '#') {
		// Comment until end of line.
		do LastChar = getNextUnicode();
		while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
		
		if (LastChar != EOF) return getTok();
	}
	
	int ThisChar = LastChar;
	LastChar = getNextUnicode();
	return ThisChar;
}
