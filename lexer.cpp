#include <iostream>
#include <cctype>
#include <vector>
#include <cassert>

enum Token {
	tok_eof = -1,

	tok_move = -2,
	tok_turn_left = -3,

	tok_if = -4,
	tok_else = -5,

	tok_for = -6,

	tok_not = -7,

	tok_front_blocked = -8,

	tok_number = -9
};

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
		output += thisChar & 0x3F;
	}

	return output;
}

int main() {
	int output;
	while (true) {
		output = getNextUnicode();
		if (output == EOF) break;
		printf("0x%X\n", output);
	}
	return 0;
}
