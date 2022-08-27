#pragma once

#include <memory>
#include <vector>
#include "lexer.hpp"

enum Movement {
	move = Token::tok_move, turn_left = Token::tok_turn_left
};

/// ExprAST - Base class for all expression nodes
class ExprAST {
public:
	virtual ~ExprAST() = default;

	// Generates the actual assembly code
	virtual void codegen(ostream &out) = 0;

	// Helper function: counts the number of instructions in codegen
	virtual int instructionCount() = 0;
	
	// Debugging function: dumps the contents of the AST in readable format
	virtual void dump(int indent) = 0;
};

/// MovementAST - Represents an movement, either move or turn left
class MovementAST : public ExprAST {
	Movement movement;
	
public:
	MovementAST(Movement movement) : movement(movement) {}
	void codegen(ostream &out) override;
	int instructionCount() override;
	void dump(int indent) override;
};

/// FrontBlockedAST - Indicating front blocked
class FrontBlockedAST : public ExprAST {
public:
	void codegen(ostream &out) override;
	int instructionCount() override;
	void dump(int indent) override;	
};

/// NotExprAST - Indicating a not on a condition
class NotExprAST : public ExprAST {
	std::unique_ptr<ExprAST> Cond;
	
public:
	NotExprAST(std::unique_ptr<ExprAST> Cond) : Cond(std::move(Cond)) {}
	void codegen(ostream &out) override;
	int instructionCount() override;
	void dump(int indent) override;
};

/// CondAST - Expression for conditions
class CondAST : public ExprAST {
	std::unique_ptr<ExprAST> Cond;
	
public:
	CondAST(std::unique_ptr<ExprAST> Cond) : Cond(std::move(Cond)) {}
	void codegen(ostream &out) override;
	int instructionCount() override;
	void dump(int indent) override;
};

/// BinaryCondAST - Expression class for binary relations aka '&' and '|'
class BinaryCondAST : public ExprAST {
	int Op;
	std::unique_ptr<ExprAST> LHS, RHS;
	
public:
	BinaryCondAST(int Op, std::unique_ptr<ExprAST> LHS, 
			std::unique_ptr<ExprAST> RHS)
		: Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
	void codegen(ostream &out) override;
	int instructionCount() override;
	void dump(int indent) override;
};

/// IfExprAST - Expression class for if/then/else
class IfExprAST : public ExprAST {
	std::unique_ptr<ExprAST> Cond;
	std::unique_ptr<ExprAST> Then, Else;
	
public:
	IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then, 
			std::unique_ptr<ExprAST> Else)
		: Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}
	void codegen(ostream &out) override;
	int instructionCount() override;
	void dump(int indent) override;
};

/// WhileLoopAST - Expression class for while loops - aka loops with a condition
class WhileLoopAST : public ExprAST {
	std::unique_ptr<ExprAST> Cond;
	std::unique_ptr<ExprAST> Body;
	
public:
	WhileLoopAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Body)
		: Cond(std::move(Cond)), Body(std::move(Body)) {}
	void codegen(ostream &out) override;
	int instructionCount() override;
	void dump(int indent) override;
};

/// ForLoopAST - Expression class indicating for loops loop over 'count' times
class ForLoopAST : public ExprAST {
	int count;
	std::unique_ptr<ExprAST> Body;

public:
	ForLoopAST(int count, std::unique_ptr<ExprAST> Body) 
		: count(count), Body(std::move(Body)) {}
	void codegen(ostream &out) override;
	int instructionCount() override;
	void dump(int indent) override;
};

/// BlockAST - AST representing an entire block of elements
class BlockAST : public ExprAST {
	std::vector<std::unique_ptr<ExprAST>> actions;
	
public:
	BlockAST(std::vector<std::unique_ptr<ExprAST>> actions) 
		: actions(std::move(actions)) {}
	void codegen(ostream &out) override;
	int instructionCount() override;
	void dump(int indent) override;
};

/// ProgramAST - AST representing a program
class ProgramAST : public ExprAST {
	std::unique_ptr<ExprAST> StartBlock;
	
public:
	ProgramAST(std::unique_ptr<ExprAST> StartBlock) 
		: StartBlock(std::move(StartBlock)) {}
	void codegen(ostream &out) override;
	int instructionCount() override;
	void dump() { dump(0); }
	void dump(int indent) override;
};

