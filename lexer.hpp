#pragma once

enum Token {
	tok_eof = -1,

	tok_move = -2,
	tok_turn_left = -3,

	tok_if = -4,
	tok_else = -5,

	tok_while = -6,

	tok_not = -7,

	tok_front_blocked = -8,

	tok_number = -9
};

// This contains the numeric value of a integer if getTok == tok_number
extern int numVal;

static int getNextUnicode();
int getTok();
