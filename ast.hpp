
/// ExprAST - Base class for all expression nodes
class ExprAST {
public:
	virtual ~ExprAST() = default;
};

class NumberAST : public ExprAST {
	int val;
	
public:
	NumberAST(int val) : val(val) {}
};
