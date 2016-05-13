#include "astnode.h"
#include <assert.h>

ASTNode::ASTNode(ASTNode * p) : parent(p), children() {
	if (parent != NULL) {
		parent->children.push_back(this);
	}
}

ASTNode::~ASTNode() {
	if (parent != NULL) {
		for (auto i = parent->children.begin();
		i != parent->children.end(); i++) {
			if (*i == parent) {
				parent->children.erase(i);
				break;
			}
		}
	}
	auto i = children.begin();
	while (i != children.end()) {
		ASTNode* toDel = *i;
		i = children.erase(i);
	}
}

//add a child to this node
void ASTNode::add(ASTNode * other) {
	assert(other != NULL);
	children.push_back(other);
	assert(other->parent == NULL);
	other->parent = this;
}

bool ASTNode::Accept(Visitor * v) {
	return v->Visit(this);
}


//utility template to typecast children nodes
template<typename T>
T ASTNode::getChild(int n) {
	return static_cast<T>(children[n]);
}


//visitor pattern overloads
bool IdentLeaf::Accept(Visitor *v) { return v->Visit(this); }
bool EXPRNode::Accept(Visitor *v) { return v->Visit(this); }
bool DECLNode::Accept(Visitor *v) { return v->Visit(this); }
bool ASSIGNNode::Accept(Visitor *v) { return v->Visit(this); }
bool FORNode::Accept(Visitor *v) { return v->Visit(this); }
bool READNode::Accept(Visitor *v) { return v->Visit(this); }
bool PRINTNode::Accept(Visitor *v) { return v->Visit(this); }
bool ASSERTNode::Accept(Visitor *v) { return v->Visit(this); }


//functions to cast and return the child nodes


EXPRNode * ASSERTNode::getExpr() {
	return getChild<EXPRNode*>(0);
}


EXPRNode * PRINTNode::getExpr() {
	return getChild<EXPRNode*>(0);
}


IdentLeaf * READNode::getIdent() {
	return getChild<IdentLeaf*>(0);
}


IdentLeaf * FORNode::getIdent() {
	return getChild<IdentLeaf*>(0);
}
EXPRNode * FORNode::getStartE() {
	return getChild<EXPRNode*>(1);
}
EXPRNode * FORNode::getEndE() {
	return getChild<EXPRNode*>(2);
}
ASTNode * FORNode::getBody() {
	return children[3];
}


IdentLeaf * ASSIGNNode::getIdent() {
	return getChild<IdentLeaf*>(0);
}
EXPRNode * ASSIGNNode::getExpr() {
	return getChild<EXPRNode*>(1);
}


IdentLeaf * DECLNode::getIdent() {
	return getChild<IdentLeaf*>(0);
}
TypeLeaf * DECLNode::getType() {
	return getChild<TypeLeaf*>(1);
}


ASTNode * EXPRNode::get1stOpnd() {
	return children[0];
}
ASTNode * EXPRNode::get2ndOpnd() {
	assert(children.size() == 2);
	return children[1];
}