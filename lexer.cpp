#include <iostream>
#include <cctype>
#include <cassert>
#include "lexer.hpp"

// Returns a 4-byte integer representing the unicode value of the next character
static int getNextUnicode() {
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
		output += thisChar & 0x3F;
	}

	return output;
}

// This contains the numeric value of a integer if getTok == tok_number
int numVal;

int getTok() {
	int thisChar = getNextUnicode();
	
	while(isspace(thisChar))
		thisChar = getNextUnicode();
	
	if (thisChar == EOF)
		return Token::tok_eof;
		
	if (isdigit(thisChar)) {
		numVal = 0;
		do {
			numVal = numVal * 10 + (thisChar - '0');
			thisChar = getNextUnicode();
		} while (isdigit(thisChar));
		
		return Token::tok_number;
	}
	
	if (thisChar == '#') {
		// Comment until end of line.
		do thisChar = getchar();
		while (thisChar != EOF && thisChar != '\n' && thisChar != '\r');
		
		if (thisChar != EOF) return getTok();
	}
	
	if (thisChar == 0x27A1) return Token::tok_move;
	if (thisChar == 0x21A9) return Token::tok_turn_left;
	if (thisChar == 0x1F914) return Token::tok_if;
	if (thisChar == 0x1F641) return Token::tok_else;
	if (thisChar == 0x1F504) return Token::tok_while;
	if (thisChar == 0x1F6AB) return Token::tok_not;
	if (thisChar == 0x1F9F1) return Token::tok_front_blocked;
}
