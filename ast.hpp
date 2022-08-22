#pragma once

#include <algorithm>

/// ExprAST - Base class for all expression nodes
class ExprAST {
public:
	virtual ~ExprAST() = default;
};

/// NumberAST - Expression class for numeric literals
class NumberAST : public ExprAST {
	int val;
	
public:
	NumberAST(int val) : val(val) {}
};

/// NotExprAST - Indicating a not on a condition
class NotExpr : public ExprAST {
	std::unique_ptr<ExprAST> condition;
	
public:
	CondAST(std::unique_ptr<ExprASTAST> condition) : condition(condition) {}
};

/// CondAST - Expression for conditions
class CondAST : public ExprAST {
	std::unique_ptr<ExprAST> condition;
	
public:
	CondAST(std::unique_ptr<ExprAST> condition) : condition(condition) {}
};

/// BinaryCondAST - Expression class for binary relations aka '&' and '|'
class BinaryCondAST : public ExprAST {
	char Op;
	std::unique_ptr<ExprAST> LHS, RHS;
	
public:
	BinaryCondAST(char Op, std::unique_ptr<ExprAST> LHS, 
			std::unique_ptr<ExprAST> RHS)
		: Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

/// IfExprAST - Expression class for if/then/else
class IfExprAST : public ExprAST {
	std::unique_ptr<ExprAST> Cond;
	std::unique_ptr<ExprAST> Then, Else;
	
public:
	IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then, 
			std::unique_ptr<ExprAST> Else)
		: Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}
};

/// WhileLoopAST - Expression class for while loops - aka loops with a condition
class WhileLoopAST : public ExprAST {
	std::unique_ptr<ExprAST> Cond;
	std::unique_ptr<ExprAST> Body;
	
public:
	WhileLoopAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Body)
		: Cond(std::move(Cond)), Body(std::move(Body)) {}
};

/// ForLoopAST - Expression class indicating for loops loop over 'count' times
class ForLoopAST : public ExprAST {
	int count;
	std::unique_ptr<ExprAST> Body;

public:
	ForLoopAST(int count, std::unique_ptr<ExprAST> Body) 
		: count(count), Body(std::move(Body)) {}
};

