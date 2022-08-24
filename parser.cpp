#include <cstdio>
#include <memory>
#include <map>
#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int CurTok;
int getNextToken() { return CurTok = getTok(); }
std::map<char, int> BinopPrecedence;

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
	
	auto Cond = ParseCond();
	if (!Cond) return nullptr;
	
	return std::make_unique<NotExprAST>(std::move(Cond));
}

/// condexpr ::= frontblkexpr
/// 		  |  notexpr
///   		  |  '(' condexpr ')'
/// 		  |  binarycond
static std::unique_ptr<ExprAST> ParseCond() {
	switch (CurTok) {
	default:
		return ParseBinaryCond();
	case Token::tok_front_blocked:
		return ParseFrontBlocked();
	case Token::tok_not:
		return ParseNot();
	case '(':
		getNextToken();	// consume '('
		std::unique_ptr<ExprAST> Cond = ParseCond();
		if (CurTok != ')') return LogError("Expected ')'");
		getNextToken();	// consume ')'
		return std::make_unique<CondAST>(std::move(Cond));
	}
}

/// binarycond ::= cond binorphs
static std::unique_ptr<ExprAST> ParseBinaryCond() {
	auto LHS = ParseCond();
	if (!LHS) return nullptr;
	return ParseBinOpRHS(0, std::move(LHS));
}

/// binoprhs ::= ('&' cond)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(
		int ExprPrec, std::unique_ptr<ExprAST> LHS) {
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
	
	// TODO: need to put these two variables in a union instead to save memory
	std::unique_ptr<ExprAST> Cond;	// used only in while loops
	int count;						// used only in for loops
	
	if (CurTok == tok_number) {
		count = numVal;
		getNextToken();	// consume the number
	}
	else {
		Cond = ParseCond();
		if (!Cond) return nullptr;
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
		return std::make_unique<ForLoopAST>(count, std::move(Body)); 
	else
		return std::make_unique<WhileLoopAST>(std::move(Cond), std::move(Body));
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
		
	std::unique_ptr<ExprAST> Action = ParseAction();
	if (!Action) return nullptr;
	
	actions.push_back(std::move(Action));
	
	while (CurTok == tok_move || CurTok == tok_turn_left 
			|| CurTok == tok_if || CurTok == tok_while) {
		Action = ParseAction();
		if (!Action) return nullptr;
		actions.push_back(std::move(Action));
	}
	
	return std::make_unique<BlockAST>(std::move(actions));
}

/// program ::= block
std::unique_ptr<ExprAST> ParseProgram() {
	if (auto B = ParseBlock()) {
		auto output = std::make_unique<ProgramAST>(std::move(B));
		if (getNextToken() == tok_eof) return output;
		else return LogError("Expected EOF");
	}
	else 
		return nullptr;
}
