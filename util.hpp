#pragma once

#include <iostream>
#include <cassert>
#include "parser.hpp"

void SetupBinops() {
	BinopPrecedence['|'] = 2;
	BinopPrecedence['&'] = 4;
}

void printToken(int token, int numVal) {
	assert(token < 128);
	assert(token >= -9);
	
	if (token >= 0) {
		std::cout << (char) token << std::endl;
		return;
	}
	
	switch (token) {
	case -1: 
		std::cout << "<<EOF>>" << std::endl;
		return;
	case -2:
		std::cout << "MOVE" << std::endl;
		return;
	case -3:
		std::cout << "TURNLEFT" << std::endl;
		return;
	case -4:
		std::cout << "IF" << std::endl;
		return;
	case -5:
		std::cout << "ELSE" << std::endl;
		return;
	case -6:
		std::cout << "WHILE" << std::endl;
		return;
	case -7:
		std::cout << "NOT" << std::endl;
		return;
	case -8:
		std::cout << "BLOCKED" << std::endl;
		return;
	case -9:
		std::cout << '[' << numVal << ']' << std::endl;
		return;
	}
}
