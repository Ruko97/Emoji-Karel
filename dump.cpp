#include <cstdio>
#include <cassert>
#include "ast.hpp"

void printIndents(int indent) {
	for (int i = 1; i <= indent; ++i) printf("\t");
}

void MovementAST::dump(int indent) {
	printIndents(indent);
	if (movement == move) printf("MOVE\n");
	else if (movement == turn_left) printf("TURNLEFT\n");
	else assert(movement == move || movement == turn_left);	// should show error
}

// No newlines are added in expressions that have boolean outputs.
// Their newlines are added by the If or while/for they are in

void FrontBlockedAST::dump(int indent) {
	printIndents(indent);
	printf("FRONTBLOCKED");
}

void NotExprAST::dump(int indent) {
	printf("NOT ");
	Cond->dump(0);	// Conditions stay on the same line as the Not, if or 
					// for/while loop they are in
}

void CondAST::dump(int indent) {
	printf("(");
	Cond->dump(0);
	printf(")");
}

void BinaryCondAST::dump(int indent) {
	LHS->dump(indent);
	printf(" %c ", Op);
	RHS->dump(indent);
}

void IfExprAST::dump(int indent) {
	printIndents(indent);
	printf("IF ");
	Cond->dump(0);
	printf("\n");
	printIndents(indent);
	printf("THEN\n");
	Then->dump(indent+1);
	printIndents(indent);
	printf("ELSE\n");
	Else->dump(indent+1);
	printIndents(indent);
	printf("ENDIF\n");
}

void WhileLoopAST::dump(int indent) {
	printIndents(indent);
	printf("WHILE ");
	Cond->dump(0);
	printf("\n");
	printIndents(indent);
	printf("DO\n");
	Body->dump(indent+1);
	printIndents(indent);
	printf("ENDWHILE\n");
}

void ForLoopAST::dump(int indent) {
	printIndents(indent);
	printf("FOR i=1..%d\n", count);
	Body->dump(indent+1);
	printIndents(indent);
	printf("ENDFOR\n");
}

void BlockAST::dump(int indent) {
	for (auto &action : actions) {
		action->dump(indent);
	}
}

void ProgramAST::dump(int indent) {
	printf("::PROGRAM::\n\n");
	StartBlock->dump(indent);
}
