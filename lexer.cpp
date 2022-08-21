#include <iostream>
#include <cctype>
#include <vector>

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

struct Lexicon {
	Token token;
	int value;	// only used if the token is tok_number
};

static enum State {
	init, move_or_turn, not_move_or_turn, control_or_blocked, not_state, number 
};

std::vector<Lexicon> LexerOutputs;

Lexicon getTok() {
	static int LastChar = ' ';
	static State current_state = init;
	int value = 0;

	while(isspace(LastChar))
		LastChar = getchar(); 
	
	if (LastChar == 0xE2) {
		if (current_state == init) {
				state = move_or_turn;
	}

	int ThisChar = LastChar;
	Lexicon output = {getchar(), 0};
	return output;
}

