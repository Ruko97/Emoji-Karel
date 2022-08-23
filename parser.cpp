#include <cstdio>
#include <memory>
#include <map>
#include "ast.hpp"
#include "lexer.hpp"

/// CurTok/getNextToken - Provides a simple token buffer. CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results
static int CurTok;
static int getNextToken() { return CurTok = getTok(); }

/// BinopPrecedence - holds the precendence for each binary operation that is defined
static std::map<char, int> BinopPrecedence;

/// GetTokPrecedence - Get the precedence of the pending binary operator token
static int GetTokPrecedence() {
	if (!isascii(CurTok)) return -1;

	// Make sure it's a declared binop
	int TokPrec = BinopPrecedence[CurTok];
	if (TokPrec <= 0) return -1;

	return TokPrec;
}

/// LogError* - These are little helper functions for error handling
static std::unique_ptr<ExprAST> LogError(const char *Str) {
	fprintf(stderr, "Error: %s\n", Str);
}

/// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr() {
	if (CurTok == '(') {
		getNextToken();	// consume '('
		
		auto result = ParseNumberExpr();
		if (!result) return nullptr;
		
		if (CurTok != ')') return LogError("Expected ')'");
		getNextToken();	// consume ')'
		
		return result;	// NOTE: consumes up the numberexpr instead
						// TODO: may need to modify this
	} else {
		auto result = std::make_unique<NumberAST>(numVal);
		getNextToken();	// consume the number
		return std::move(result);
	}
}

/// movementexpr ::= ➡ 
///               |  ↩
static std::unique_ptr<ExprAST> ParseMovement() {
	Movement movement;
	if (CurTok == Token::tok_move) movement = move;
	if (CurTok == Token::tok_turn_left) movement = turn_left;
	else return LogError("Expected '➡' or '↩'");
	
	auto result = std::make_unique<MovementAST>(movement);
	getNextToken(); // consume the movmement token
	return std::move(result);
};

/// frontblkexpr ::= 🧱
static std::unique_ptr<ExprAST> ParseFrontBlocked() {
	if (CurTok != Token::tok_front_blocked)
		return LogError("Expected 🧱");
		
	auto result = std::make_unique<FrontBlockedAST>();
	getNextToken(); // consume 🧱
	return std::move(result);
}

/// notexpr ::= 🚫 conditional
static std::unique_ptr<ExprAST> ParseNot() {
	if (CurTok != Token::tok_not) return LogError("Expected 🚫");
	getNextToken();	// consume 🚫
	
	auto Cond = parseCond();
	if (!Cond) return nullptr;
	
	return std::make_unique<NotExprAST>(Cond);
}

/// condexpr ::= frontblkexpr
static std::unique_ptr<ExprAST> ParseCond() {
	switch (CurTok) {
		case Token::tok_front_blocked:
			return ParseFrontBlocked();
		case Token::tok_not:
			return ParseNot();
		case '(':
			getNextToken();	// consume '('
			auto Cond = ParseCond();
			if (CurTok != ')') return LogError("Expected ')'");
			getNextToken();	// consume ')'
			return std::make_unique<CondAST>(Cond);
		default:
			return ParseBinaryCond();
	}
}
