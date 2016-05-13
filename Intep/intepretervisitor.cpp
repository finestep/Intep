#include "intepretervisitor.h"

#include "util.h"


bool IntepreterVisitor::Visit(DECLNode * node)
{
	return true;
}


bool IntepreterVisitor::Visit(ASSIGNNode * node)
{
	_symbols->setSym(_symbols->getSym(node->getIdent()->ident),
		eval(node->getExpr() ).val );
	return true;
}

#define SIGN(x) ( (x > 0) - (x < 0) )

bool IntepreterVisitor::Visit(FORNode * node)
{
	symindex symI = _symbols->getSym(node->getIdent()->ident  );
	assert(symI != SymTable::NO_SYM); //this should be prevented by semantic analysis
	Symbol var = _symbols->getSym(symI);

	int start = eval(node->getStartE()).val.integer;
	int end = eval(node->getEndE()).val.integer;
	int dir = SIGN(end - start);

	for (var.val.integer = start; var.val.integer != end+dir; 
		var.val.integer+= dir) { //direction of iteration
		_symbols->setSym(symI, var.val);
		if ( !node->getBody()->Accept(this) ) return false;
	}
	_symbols->setSym(symI, var.val);
	return true;
}


//A helper function to query stdin input
bool IntepreterVisitor::getInput(symindex index) {
	std::stringstream buf;
	char c;
	Symbol s = _symbols->getSym(index);

	//help the user input the correct type
	std::cout << '(' << Type_str[s.type] << ") >";


	while ( !isWhitespace(std::cin.peek()) && std::cin.peek()!=EOF) {
		c = std::cin.get();
		buf << c;
	}

	s = _symbols->getSym(index);
	if (s.type == T_INT) {
		_symbols->setSym(index, 
			makeIntVal(strtol( buf.str().c_str(),NULL,10 ) ) );
		return true;
	}
	else if (s.type == T_STRING) {
		s.val.str->assign( buf.str() );
		return true;
	}

	return false;
}

bool IntepreterVisitor::Visit(READNode * node) {

	symindex symI = _symbols->getSym(node->getIdent()->ident);
	assert(symI != SymTable::NO_SYM);
	Symbol sym = _symbols->getSym(symI);

	return getInput(symI);
}

bool IntepreterVisitor::Visit(PRINTNode * node) {

	Symbol v = eval( node->getExpr() );
	if (v.type == T_STRING) std::cout << *v.val.str;
	else std::cout << v.val.integer;
	return true;
}

bool IntepreterVisitor::Visit(ASSERTNode * node)
{
	if (!eval(node->getExpr()).val.boolean) {
		std::cout << "ASSERT FAILED AT " << node->origin << '\n';
	}
	return true;
}

bool IntepreterVisitor::Visit(EXPRNode * node)
{
	return false;
}

//evaluates an expression subnode
//node type is determined by repeated casting :(
Symbol IntepreterVisitor::eval(ASTNode* node) {
	IntLeaf *intNode = dynamic_cast<IntLeaf*>(node);

	if (intNode != NULL) {
		return Symbol(T_INT,makeIntVal(intNode->val) );
	}

	StrLeaf *strNode = dynamic_cast<StrLeaf*>(node);
	if (strNode != NULL) {
		return Symbol(T_STRING, makeStrVal(strNode->str) );
	}

	IdentLeaf *identNode = dynamic_cast<IdentLeaf*>(node);
	if (identNode != NULL) {
		symindex symI = _symbols->getSym(identNode->ident);
		assert(symI != SymTable::NO_SYM);
		return _symbols->getSym(symI);
	}

	EXPRNode* expr = dynamic_cast<EXPRNode*>(node);
	if (expr != NULL) {
		SymVal v1 = eval(expr->get1stOpnd()).val;
		SymVal v2;
		if (expr->type != OP_NOT) v2 = eval(expr->get2ndOpnd()).val;
		std::string *s;
		switch (expr->type) {
		case OP_MINUS:
			return Symbol(T_INT,makeIntVal(v1.integer - v2.integer) );
			break;
		case OP_MULT:
			return Symbol(T_INT, makeIntVal(v1.integer * v2.integer) );
			break;
		case OP_DIV:
			return Symbol(T_INT, makeIntVal(v1.integer / v2.integer) );
			break;
		case OP_PLUS:
			return Symbol(T_INT, makeIntVal(v1.integer + v2.integer) );
			break;
		case OP_CONCAT:
			//NEW STRING ALLOCATED
			//TODO figure out how to free it
			s = new std::string(*v1.str);
			s->append(*v2.str);
			return Symbol(T_STRING, makeStrVal( s ) );
		case OP_AND:
			return Symbol(T_BOOL, makeBoolVal(v1.boolean && v2.boolean) );
		case OP_EQ:
			return Symbol(T_BOOL, makeBoolVal(v1.integer == v2.integer) );
		case OP_STR_EQ:
			return Symbol(T_BOOL, makeBoolVal(*v1.str == *v2.str) );
		case OP_LESS:
			return Symbol(T_BOOL, makeBoolVal(v1.integer < v2.integer) );
		case OP_STR_LESS:
			return Symbol(T_BOOL, makeBoolVal(*v1.str < *v2.str) );
		case OP_NOT:
			return Symbol(T_BOOL, makeBoolVal(!v1.boolean) );
		}
		assert(false); //no longer a valid program state
	}

	assert(false);
}