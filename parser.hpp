#pragma once

#include <cstdio>
#include <memory>
#include <map>
#include "lexer.hpp"
#include "ast.hpp"

/// CurTok/getNextToken - Provides a simple token buffer. CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results
extern int CurTok;
int getNextToken();

/// BinopPrecedence - holds the precendence for each binary operation that is defined
extern std::map<char, int> BinopPrecedence;

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

/// Representing the type of loop in parseLoop
enum LoopType { for_loop, while_loop };

/// struct with a union that stores either count (in for-loop) or 
/// boolean condition (in while-loop)
struct LoopCond {
	union {
		std::unique_ptr<ExprAST> Cond;
		int count;
	};
};

// The different parse functions 

static std::unique_ptr<ExprAST> ParseMovement();
static std::unique_ptr<ExprAST> ParseFrontBlocked();
static std::unique_ptr<ExprAST> ParseNot();
static std::unique_ptr<ExprAST> ParseCond();
static std::unique_ptr<ExprAST> ParseBinaryCond();
static std::unique_ptr<ExprAST> 
		ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
static std::unique_ptr<ExprAST> ParseIfStmt();
static std::unique_ptr<ExprAST> ParseLoop();
static std::unique_ptr<ExprAST> ParseAction();
static std::unique_ptr<ExprAST> ParseBlock();

std::unique_ptr<ExprAST> ParseProgram();
