#pragma once

#include <unordered_set>
#include "filepos.h"
#include "visitor.h"

//Populates the symbol table and verifies the
//semantic constraints of the Mini-PL AST
class SemanticVisitor : public Visitor {

	EType exprType(ASTNode* node); 


	std::unordered_set<symindex> _constants; //should be at symtable?
public:
	SemanticVisitor(SymTable *sym) : Visitor(sym) {}

	inline virtual bool Visit(ASTNode* node) {
		return Visitor::Visit(node);
	}

	virtual bool Visit(DECLNode* node);
	virtual bool Visit(ASSIGNNode* node);
	virtual bool Visit(FORNode* node);
	virtual bool Visit(READNode* node);
	virtual bool Visit(PRINTNode* node);
	virtual bool Visit(ASSERTNode* node);

	virtual bool Visit(EXPRNode* node);
};