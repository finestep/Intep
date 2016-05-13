#include "semanticvisitor.h"

#include "astnode.h"

//must only declare an identifier once
bool SemanticVisitor::Visit(DECLNode * node)
{
	if (_symbols->getSym(node->getIdent()->ident) == SymTable::NO_SYM) {

		_symbols->addSym(node->getIdent()->ident, node->getType()->type);

		return true;
	}

	std::cout << "Symbol " << *node->getIdent()->ident
		<< " redeclared at " << node->origin << '\n';

	return false;
}


bool SemanticVisitor::Visit(ASSIGNNode * node)
{
	symindex index = _symbols->getSym(node->getIdent()->ident);
	if (index == SymTable::NO_SYM) {
		std::cout << "Symbol " << *node->getIdent()->ident
		<< " used before defined at " << node->origin << '\n';
		return false;
	}

	if (_constants.find(index) != _constants.end()) {
		std::cout << "Attempt to assign constant symbol "
			<< *node->getIdent()->ident
			<< " at " << node->origin << '\n';
		return false;
	}

	Symbol sym = _symbols->getSym(index);
	EType t = exprType(node->getExpr());
	if (sym.type != t) {
		std::cout << "Attempt to assign " << Type_str[t]
			<< " to symbol " << *node->getIdent()->ident
			<< " of type " << Type_str[sym.type]
			<< " at " << node->getExpr()->origin << '\n' ;
		return false;
	}

	return true;
}

bool SemanticVisitor::Visit(FORNode * node)
{
	symindex index = _symbols->getSym(node->getIdent()->ident);

	if (index == SymTable::NO_SYM) return false;
	Symbol sym = _symbols->getSym(index);
	if ((sym.type) != T_INT) {
		std::cout << node->getIdent()->origin
			<< ": Symbol " << *node->getIdent()->ident
			<< " must be of integer type\n";
		return false;
	}

	if (exprType(node->getStartE()) != T_INT) {
		std::cout << "Expression at " << node->getStartE()->origin
			<< " is not an integer\n";
		return false;
	}
	if (exprType(node->getEndE()) != T_INT) {
		std::cout << "Expression at " << node->getEndE()->origin
			<< " is not an integer\n";
		return false;
	}


	_constants.insert( _symbols->getSym( node->getIdent()->ident ));
	bool valid = Visitor::Visit(node->getBody());
	if (!valid) return false;
	_constants.erase(_symbols->getSym(node->getIdent()->ident));
	return true;
}

bool SemanticVisitor::Visit(READNode * node)
{
	symindex index = _symbols->getSym(node->getIdent()->ident);
	if (index == SymTable::NO_SYM) {
		std::cout << node->getIdent()->origin
			<< ": Symbol " << *node->getIdent()->ident
			<< " is not declared\n";
		return false;
	}
		
	if (_constants.find(index) != _constants.end()) {
		std::cout << "Attempt to assign constant symbol "
			<< *node->getIdent()->ident
			<< " at " << node->origin << '\n';
	}
	return true;
}

bool SemanticVisitor::Visit(PRINTNode * node)
{
	EType t = exprType(node->getExpr() );
	if (t != T_INT && t != T_STRING) {
		std::cout << "Expression at " << node->getExpr()->origin
			<< " is not printable\n";
		return false;
	}

	return true;
}

bool SemanticVisitor::Visit(ASSERTNode * node)
{
	if (exprType(node->getExpr()) != T_BOOL) {
		std::cout << "Expression at " << node->getExpr()->origin
			<< " is not a boolean\n";
		return false;
	}
}

bool SemanticVisitor::Visit(EXPRNode * node)
{
	EType t1 = exprType(node->get1stOpnd());
	EType t2 = T_NONE;
	if (node->type != OP_NOT) t2 = exprType(node->get2ndOpnd());

	switch (node->type) {
	case OP_MINUS:
	case OP_MULT:
	case OP_DIV:
		if (t1 != T_INT || t2 != T_INT) return false;
		break;
	case OP_PLUS:
		if (t1 != t2 || (t1 != T_INT && t1 != T_STRING) ||
			(t2 != T_INT && t2 != T_STRING)) return false;

		break;

	case OP_AND:
		if (t1 != T_BOOL || t2 != T_BOOL) return false;
		break;
	case OP_EQ:
	case OP_LESS:
		if (t1 != t2) return false;
		break;
	case OP_NOT:
		if (t1 != T_BOOL) return false;

	}

	return node->get1stOpnd()->Accept(this)
		&& node->get2ndOpnd()->Accept(this);
}

//TODO decorate tree instead?
EType SemanticVisitor::exprType(ASTNode* node) {
	IntLeaf *intNode = dynamic_cast<IntLeaf*>(node);
	if (intNode != NULL)
		return T_INT;

	StrLeaf *str = dynamic_cast<StrLeaf*>(node);
	if (str != NULL)
		return T_STRING;

	IdentLeaf *identNode = dynamic_cast<IdentLeaf*>(node);
	if (identNode != NULL) {
		symindex symI = _symbols->getSym(identNode->ident);
		if (symI == SymTable::NO_SYM) {
			std::cout << identNode->origin
				<< ": Symbol " << *identNode->ident
				<< " is not declared\n";
			return T_NONE;
		}
		return _symbols->getSym(symI).type;
	}

	EXPRNode* expr = dynamic_cast<EXPRNode*>(node);
	if (expr != NULL) {
		EType t1 = exprType(expr->get1stOpnd());
		EType t2 = T_NONE;
		if (expr->type != OP_NOT) t2 = exprType(expr->get2ndOpnd());
		
		switch (expr->type) {

			case OP_MINUS:
			case OP_MULT:
			case OP_DIV:
				if (t1 == T_INT && t2 == T_INT) return T_INT;
				break;

			case OP_PLUS: //if operands are strings, transform into concatenation operator
				if (t1 != t2) break;
				if(t1 == T_STRING ) {
					expr->type = OP_CONCAT;
					return T_STRING;
				}
				else if (t1 == T_INT) {
					return T_INT;
				}
				break;

			case OP_AND:
				if (t1 == T_BOOL && t2 == T_BOOL) return T_BOOL;
				break;

			case OP_EQ:
				if (t1 != t2) break;
				if(t1 == T_STRING) { //string equality
					expr->type = OP_STR_EQ;
					return T_BOOL;
				}
				if (t1 == T_BOOL) return T_BOOL;

			case OP_LESS:
				if (t1 != t2) break;
				if (t1 == T_STRING) { //string order
					expr->type = OP_STR_LESS;
					return T_BOOL;
				}
				if (t1 == T_BOOL) return T_BOOL;
				break;

			case OP_NOT:
				if (t1 == T_BOOL) return T_BOOL;
		}
		std::cout << "Invalid operation at " << node->origin;
		return T_NONE;
	}

	return T_NONE;

}