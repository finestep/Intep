#pragma once


#include "symtable.h"
#include "astnode.h"

//forward declarations 

struct ASTNode;
struct DECLNode;
struct ASSIGNNode;
struct FORNode;
struct READNode;
struct PRINTNode;
struct ASSERTNode;
struct EXPRNode;
struct IdentLeaf;


class Visitor {
protected:
	SymTable *_symbols;
public:
	Visitor(SymTable *sym) : _symbols(sym) {}

	bool Visit(ASTNode* node);

	virtual bool Visit(DECLNode* node) = 0;
	virtual bool Visit(ASSIGNNode* node) = 0;
	virtual bool Visit(FORNode* node) = 0;
	virtual bool Visit(READNode* node) = 0;
	virtual bool Visit(PRINTNode* node) = 0;
	virtual bool Visit(ASSERTNode* node) = 0;

	virtual bool Visit(EXPRNode* node) = 0;

};