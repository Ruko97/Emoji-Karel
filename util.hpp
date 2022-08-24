#pragma once

#include "parser.hpp"

void SetupBinops() {
	BinopPrecedence['|'] = 2;
	BinopPrecedence['&'] = 4;
}
