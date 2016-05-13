#pragma once

#include <vector>
#include <iostream>

#include "symtable.h"
#include "visitor.h"

#include "Buffer.h"  //filepos

class Visitor;

/*

Defines the following structs

ASTNode

IntLeaf
IdentLeaf
StrLeaf

EXPRNode

STMTNode

DECLNode
ASSIGNNode
FORNode
READNode
PRINTNode
ASSERTNode

*/


/*
The data structures representing
nodes of the abstract syntax tree
*/

struct ASTNode {
protected:
	ASTNode *parent; //never actually used
	template <typename T> T getChild(int n);
public:
	ASTNode(ASTNode* p = NULL);
	~ASTNode();
	void add(ASTNode* other);
	virtual bool Accept(Visitor *v);
	std::vector<ASTNode*> children;
	filepos origin;
	inline virtual void print(int level=0) {
		for (auto i = children.begin();
					i != children.end(); i++) {
			for (int i = 0; i < level; i++) std::cout << "\t" << (i%2==1?"|":"");
			(*i)->print(level+1);
		}
	}
};



struct TypeLeaf : public ASTNode {
	EType type;
	TypeLeaf(EType t, ASTNode* parent = NULL) : ASTNode(parent), type(t) {}


	inline virtual void print(int level) {
		std::cout << "< TypeLeaf :" << Type_str[type] << " >\n";
	}

};

struct IntLeaf : public ASTNode {
	int val;
	IntLeaf(int v, ASTNode* parent = NULL) : ASTNode(parent), val(v) {};


	inline virtual void print(int level) {
		std::cout << "< IntLeaf :" << val << " >\n";
	}
};

struct StrLeaf : public ASTNode {
	std::string *str;
	StrLeaf(std::string *s, ASTNode* parent = NULL) : ASTNode(parent), str(s) {
	}
	~StrLeaf() {
		delete str;
	}
	inline virtual void print(int level) {
		std::cout << "< StrLeaf :" << *str << " >\n";
	}
};



struct IdentLeaf : public ASTNode {
	std::string *ident;
	IdentLeaf(std::string *s, ASTNode* parent = NULL) : ASTNode(parent), ident(s) {}
	~IdentLeaf() {
		delete ident;
	}

	virtual bool Accept(Visitor *v);

	inline virtual void print(int level) {
		std::cout << "< IdentLeaf :" << *ident << " >\n";
	}
};


struct EXPRNode : public ASTNode {
	EOper type;
	EXPRNode(EOper t, ASTNode* parent = NULL) : ASTNode(parent), type(t) {}
	ASTNode* get1stOpnd();
	ASTNode* get2ndOpnd();

	virtual bool Accept(Visitor *v);

	inline virtual void print(int level) {
		std::cout << "< ExprLeaf :" << Op_str[type] << " >\n";
		ASTNode::print(level+1);
	}
};




struct STMTNode : public ASTNode {
	STMTNode(ASTNode* parent) : ASTNode(parent) {}
};



struct DECLNode : public STMTNode {

	DECLNode(ASTNode* parent = NULL) : STMTNode(parent) {}
	IdentLeaf* getIdent();
	TypeLeaf* getType();

	virtual bool Accept(Visitor *v);

	inline virtual void print(int level) {
		std::cout << "< DECLNode >\n";
		ASTNode::print(level + 1);
	}
};

struct ASSIGNNode : public STMTNode {

	ASSIGNNode(ASTNode* parent = NULL) : STMTNode(parent) {}
	IdentLeaf* getIdent();
	EXPRNode* getExpr();

	virtual bool Accept(Visitor *v);

	inline virtual void print(int level) {
		std::cout << "< ASSIGNNode >\n";
		ASTNode::print(level + 1);
	}
};



struct FORNode : public STMTNode {
	FORNode(ASTNode* parent = NULL) : STMTNode(parent) {}

	IdentLeaf* getIdent();
	EXPRNode* getStartE();
	EXPRNode* getEndE();
	ASTNode* getBody();

	virtual bool Accept(Visitor *v);

	inline virtual void print(int level) {
		std::cout << "< FORNode >\n";
		ASTNode::print(level + 1);
	}
};

struct READNode : public STMTNode {
	READNode(ASTNode* parent = NULL) : STMTNode(parent) {}
	IdentLeaf* getIdent();

	virtual bool Accept(Visitor *v);

	inline virtual void print(int level) {
		std::cout << "< READNode >\n";
		ASTNode::print(level + 1);
	}
};



struct PRINTNode : public STMTNode {
	PRINTNode(ASTNode* parent = NULL) : STMTNode(parent) {}
	EXPRNode* getExpr();

	virtual bool Accept(Visitor *v);

	inline virtual void print(int level) {
		std::cout << "< PRINTNode >\n";
		ASTNode::print(level + 1);
	}
};



struct ASSERTNode : public STMTNode {
	ASSERTNode(ASTNode* parent = NULL) : STMTNode(parent) {}
	EXPRNode* getExpr();
	
	virtual bool Accept(Visitor *v);
	
	inline virtual void print(int level) {
		std::cout << "< ASSERTNode >\n";
		ASTNode::print(level + 1);
	}
};