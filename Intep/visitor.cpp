#pragma once

#include "visitor.h"

//iterate all children, even if some are not accepted
bool Visitor::Visit(ASTNode* node) {
	bool accept = true;
	for (auto i = node->children.begin();
	i != node->children.end(); i++) {
		if (!(*i)->Accept(this)) accept = false;
	}
	return accept;
}
