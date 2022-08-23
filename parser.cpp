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

/// movementstmt ::= ➡ 
///               |  ↩
static std::unique_ptr<ExprAST> ParseMovement() {
	std::unique_ptr<ExprAST> result;
	
	if (CurTok == Token::tok_move) 
		result = std::make_unique<MovementAST>(move);
	else if (CurTok == Token::tok_turn_left) 
		result = std::make_unique<MovementAST>(turn_left);
	else 
		return LogError("Expected '➡' or '↩'");
	
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
	
	return std::make_unique<NotExprAST>(std::move(Cond));
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
			return std::make_unique<CondAST>(std::move(Cond));
		default:
			return ParseBinaryCond();
	}
}

/// binarycond ::= cond binorphs
static std::unique_ptr<ExprAST> ParseBinaryCond() {
	auto LHS = ParseCond();
	if (!LHS) return nullptr;
	return ParseBinOpRHS(0, std::move(LHS));
}

/// binoprhs ::= ('&' cond)*
static std::unique_ptr<ExprAST> 
ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
	while (true) {
		int TokPrec = GetTokPrecedence();
		
		if (TokPrec < ExprPrec) return LHS;
		
		int BinOp = CurTok;
		getNextToken();	// consume binary operator
		
		auto RHS = ParseCond();
		if (!RHS) return nullptr;
		
		int NextPrec = GetTokPrecedence();
		if (TokPrec < NextPrec) {
			RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
			if (!RHS) return nullptr;
		}
		
		LHS = std::make_unique<BinaryCondAST>(
				BinOp, std::move(LHS), std::move(RHS));
	}
}

/// ifstmt ::= 🤔 '(' conditional ')' '{' block '}'
/// 		|  🤔 '(' conditional ')' '{' block '}' 🙁 '{' block '}' 
static std::unique_ptr<ExprAST> ParseIfStmt() {
	getNextToken(); // consume the 🤔

	if (CurTok != '(') return LogError("Expected '('");
	getNextToken();	// consume '('
	
	auto Cond = ParseCond();
	if (!Cond) return nullptr;

	if (CurTok != ')') return LogError("Expected ')'");
	getNextToken(); // consume ')'
	
	if (CurTok != '{') return LogError("Expected '{'");
	getNextToken(); // consume '{'
	
	auto Then = ParseBlock();
	if (!Then) return nullptr;
	
	if (CurTok != '}') return LogError("Expected '}'");
	getNextToken(); // consume '}'
	
	// Now we choose if there is an else or not
	if (CurTok != tok_else) 
		return std::make_unique<IfExprAST>(
				std::move(Cond), std::move(Then), nullptr);
	
	getNextToken(); // consume 🙁
	
	if (CurTok != '{') return LogError("Expected '{'");
	getNextToken(); // consume '{'
	
	auto Else = ParseBlock();
	if (!Else) return nullptr;

	if (CurTok != '}') return LogError("Expected '}'");
	getNextToken(); // consume '}'
	
	return std::make_unique<IfExprAST>(
			std::move(Cond), std::move(Then), std::move(Else));
}

/// loopstmt ::= 🔄 '(' condition ')' '{' block '}'
///           |  🔄 '(' tok_number ')' '{' block '}'
static std::unique_ptr<ExprAST> ParseLoop() {
	getNextToken();	// consume 🔄
	
	if (CurTok != '(') return LogError("Expected '('");
	getNextToken();	// consume '('
	
	union {
		std::unique_ptr<ExprAST> Cond;
		int count;
	} loopCond;
	
	if (CurTok == tok_number) {
		loopCond.count = numVal;
		getNextToken();	// consume the number
	}
	else {
		loopCond.Cond = ParseCond();
		if (!loopCond.Cond) return nullptr;
	}

	if (CurTok != ')') return LogError("Expected ')'");
	getNextToken(); // consume ')'
	
	if (CurTok != '{') return LogError("Expected '{'");
	getNextToken(); // consume '{'
	
	auto Body = ParseBlock();
	if (!Body) return nullptr;
	
	if (CurTok != '}') return LogError("Expected '}'");
	getNextToken(); // consume '}'
	
	if (CurTok == tok_number)
		return std::make_unique<ForLoopAST>(loopCond.count, std::move(Body)); 
	else
		return std::make_unique<WhileLoopAST>(loopCond.Cond, std::move(Body));
}

/// action ::= movementstmt 
///  		|  ifstmt 
/// 		|  loopstmt
static std::unique_ptr<ExprAST> ParseAction() {
	switch (CurTok) {
	default:
		return LogError("unknown token while expecting an action");
	case tok_move: case tok_turn_left:
		return ParseMovement();
	case tok_if:
		return ParseIfStmt();
	case tok_while:
		return ParseLoop();
	}
}

/// block ::= action +
static std::unique_ptr<ExprAST> ParseBlock() {
	std::vector<std::unique_ptr<ExprAST>> actions;
		
	auto Action = ParseAction();
	if (!Action) return nullptr;
	
	actions.push_back(Action);
	
	while (CurTok == tok_move || CurTok == tok_turn_left 
			|| CurTok == tok_if || CurTok == tok_while) {
		Action = ParseAction();
		if (!Action) return nullptr;
		actions.push_back(Action);
	}
	
	return std::make_unique<BlockAST>(std::move(actions));
}

/// program ::= block
static std::unique_ptr<ExprAST> ParseProgram() {
	if (auto B = ParseBlock()) 
		return std::make_unique<ProgramAST>(std::move(B));
	else 
		return nullptr;
}
