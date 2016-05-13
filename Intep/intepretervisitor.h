#pragma once

#include "visitor.h"
#include "symtable.h"

//A Visitor pattern that inteprets a valid AST

class IntepreterVisitor : public Visitor {
	Symbol eval(ASTNode* e);
	bool getInput(symindex index);
public:
	IntepreterVisitor(SymTable *sym) : Visitor(sym) {}
	// Inherited via Visitor
	inline virtual bool Visit(ASTNode* node) {
		return Visitor::Visit(node);
	}

	virtual bool Visit(DECLNode * node) override;
	virtual bool Visit(ASSIGNNode * node) override;
	virtual bool Visit(FORNode * node) override;
	virtual bool Visit(READNode * node) override;
	virtual bool Visit(PRINTNode * node) override;
	virtual bool Visit(ASSERTNode * node) override;
	virtual bool Visit(EXPRNode * node) override;
};