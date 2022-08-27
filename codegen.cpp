#include <iostream>
#include <cassert>
#include "ast.hpp"

/************************************************************/
/* Instruction Count										*/
/************************************************************/
int MovementAST::instructionCount() { return 1; }
int FrontBlockedAST::instructionCount() { return 1; }
int NotExprAST::instructionCount() { return Cond->instructionCount() + 1; }
int CondAST::instructionCount() { return Cond->instructionCount(); }

int BinaryCondAST::instructionCount() {
	return LHS->instructionCount() + RHS->instructionCount() + 3;
}

int IfExprAST::instructionCount() {
	return Cond->instructionCount() + 2 + Then->instructionCount()
			+ Else->instructionCount();
}

int WhileLoopAST::instructionCount() {
	return Cond->instructionCount() + Body->instructionCount() + 2;
}

int ForLoopAST::instructionCount() {
  // +1 for PUSHCOUNT, +1 for JCE, +1 for INC, +1 for JMP and +1 for POPCOUNT
  return Body->instructionCount() + 5;
}

int BlockAST::instructionCount() {
	int result = 0;
	for (auto &action : actions) {
		result += action->instructionCount();
	}
	return result;
}

int ProgramAST::instructionCount() {
	return StartBlock->instructionCount() + 2;
}

/************************************************************/
/* Codegen				 								  	*/
/************************************************************/

// TODO: add enums in codegen instead of using strings

void MovementAST::codegen(std::ostream &out) {
	if (movement == move)
		out << "MOVE" << std::endl;
	else if (movement == turn_left)
		out << "TURNLEFT" << std::endl;
	else assert(movement == move || movement == turn_left);	// should show error
}

void FrontBlockedAST::codegen(std::ostream &out) {
	out << "FRONTBLOCKED" << std::endl;	// After running FRONTBLOCKED instruction,
										// the accumulator should contain 1 if
										// front is really blocked, and 0 otherwise
}

void NotExprAST::codegen(std::ostream &out) {
	Cond->codegen(out);
	out << "NOT" << std::endl;	// NOT instruction inverts value in accumulator
}

void CondAST::codegen(std::ostream &out) {
	Cond->codegen(out);
}

void BinaryCondAST::codegen(std::ostream &out) {
	LHS->codegen(out);
	out << "PUSH" << std::endl;	// PUSH instruction pushes accumulator in stack
	RHS->codegen(out);
	if (Op == '&')
		out << "AND" << std::endl;	// AND instruction runs logical-and operator
									// on accumulator and top of stack
	else if (Op == '|')
		out << "OR" << std::endl;	// OR instrucution runs logical-or operator
									// on accumulator and top of stack
	else assert(Op == '&' || Op == '|');	// should show error
	out << "POP" << std::endl;	// POP instruction pops value from top of stack
}

void IfExprAST::codegen(std::ostream &out) {
	Cond->codegen(out);
	// If Cond == 0, skips instructions of 'Then'
	out << "JZ " << Then->instructionCount() << std::endl;
	Then->codegen(out);
	// Jumps over 'Else's instructions
	out << "JMP " << Else->instructionCount() << std::endl;
	Else->codegen(out);
}

void WhileLoopAST::codegen(std::ostream &out) {
	Cond->codegen(out);
	out << "JZ " << Body->instructionCount() + 2 << std::endl;
	Body->codegen(out);
	out << "JMP "
		<< -(Body->instructionCount() + 1 + Cond->instructionCount())
		<< std::endl;
}

void ForLoopAST::codegen(std::ostream &out) {
  // We use a separate stack of counter registers to store count of each
  // for loop. PUSHCOUNT instruction pushes the current counter register on
  // its own stack and sets counter register = 0
	out << "PUSHCOUNT" << std::endl;

	// JCE (jump counter equal): JCE count offset
	// Checks if the value in the current counter register == count and jumps
	// 'jumpoffset' instructions if they are equal
	out << "JCE " << count << " " << Body->instructionCount() + 3 << std::endl;

	Body->codegen(out);
	out << "INC" << std::endl;			// increments value of counter register
	out << "JMP "
		<< -(Body->instructionCount() + 2)
		<< std::endl;					// +1 for INC, +1 for JCE

  // sets counter register's value to the value on top of stack and pops
  // one value from stack
	out << "POPCOUNT" << std::endl;
}

void BlockAST::codegen(std::ostream &out) {
	for (auto &action : actions) {
		action->codegen(out);
	}
}

void ProgramAST::codegen(std::ostream &out) {
	out << "START" << std::endl;	// Indicating start of program
	StartBlock->codegen(out);
	out << "END" << std::endl;		// Indicating end of program
}
