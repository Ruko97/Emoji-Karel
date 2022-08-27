#include <iostream>
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

int ForLoopAST::instructionCount() { return Body->instructionCount() + 8; }d

int BlockLoopAST::instructionCount() {
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

void MovementAST::codegen(ostream &out) {
	if (movement == move) 
		out << "MOVE" << std::endl;
	else if (movment == turn_left) 
		out << "TURNLEFT" << std::endl;
	else assert(movement == move || movement == turn_left);	// should show error
}

void FrontBlockedAST::codegen(ostream &out) {
	out << "FRONTBLOCKED" << std::endl;	// After running FRONTBLOCKED instruction,
										// the accumulator should contain 1 if
										// front is really blocked, and 0 otherwise
}

void NotExprAST::codegen(ostream &out) {
	Cond->codegen(out);
	out << "NOT" << std::endl;	// NOT instruction inverts value in accumulator
}

void CondAST::codegen(ostream &out) {
	Cond->codegen(out);	
}

void BinaryCondAST::codegen(ostream &out) {
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

void IfExprAST::codegen(ostream &out) {
	Cond->codegen(out);
	// If Cond == 0, skips instructions of 'Then'
	out << "JZ " << Then->instructionCount() << std::endl;	
	Then->codegen(out);
	// Jumps over 'Else's instructions
	out << "JMP " << Else->instructionCount() << std::endl;	
	Else->codegen(out);
}

void WhileLoopAST::codegen(ostream &out) {
	Cond->codegen(out);
	out << "JZ " << Body->instructionCount() + 1 << std::endl;
	Body->codegen(out);
	out << "JMP " 
		<< -(Body->instructionCount() + 1 + Cond->instructionCount())
		<< std::endl;
}

void ForLoopAST::codegen(ostream &out) {
	out << "PUSH" << std::endl;			// Push current value of acc into stack
	out << "SET 0" << std::endl;		// Set acc = 0
	
	// CE (check equal) instruction checks if acc's value is equal to count's
	// and sets acc=1 if they are equal (and 0 otherwise)
	out << "CE " << count << std::endl;	 
	out << "JNE " << Body->instructionCount() + 1 << std::endl;	

	Body->codegen(out);
	out << "INC" << std::endl;			// increments value on top of stack by 1	
	out << "JMP " 
		<< -(Body->instructionCount() + 3) 
		<< std::endl;					// +1 for INC, +1 for JNE and +1 for CE

	out << "LTS" << std::endl;			// loads top of stack to accumulator
	out << "POP" << std::endl;			// pops top of stack
}

void BlockLoopAST::codegen(ostream &out) {
	for (auto &action : actions) {
		action->codegen(out);
	}	
}

void ProgramAST::codegen(ostream &out) {
	out << "START" << std::endl;	// Indicating start of program
	StartBlock->codegen(out);
	out << "END" << std::endl;		// Indicating end of program
}
