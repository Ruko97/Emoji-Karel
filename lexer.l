
%{
	const int  tok_eof = -1;

	const int tok_move = -2;
	const int tok_turn_left = -3;

	const int tok_if = -4;
	const int tok_else = -5;
	const int tok_while = -6;

	const int tok_not = -7;

	const int tok_front_blocked = -8;

	const int tok_number = -9;
%}

MOVE			\xE2\x9E\xA1\xEF\xB8\x8F
TURN_LEFT		\xE2\x86\xA9\xEF\xB8\x8F
IF				\xF0\x9F\xA4\x94
ELSE			\xF0\x9F\x99\x81
WHILE			\xF0\x9F\x94\x84
NOT				\xF0\x9F\x9A\xAB
FRONT_BLKD		\xF0\x9F\xA7\xB1
NUMBER			[0-9]+
WHITESPACE		[ \t\n]

%%

{MOVE}			{ return (int) Token.tok_eof; }
{TURN_LEFT}		{ return (int) 

%%


