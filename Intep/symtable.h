#pragma once

#include <assert.h>
#include <unordered_map>
#include <vector>
#include "token.h" //Symval

struct Symbol {
	EType type;
	SymVal val;
	Symbol(EType t): type(t)  {
		if (t == INT) {
			val.integer = 0;
		}
		else if (t == STR) {
			val.str = new std::string();
		}
		else {
			val.boolean = false;
		}

	}

	Symbol(EType t, SymVal v) : type(t), val(v) {}
};

//the numerical symbol identifier type
typedef int symindex;

//internal types used by the SymTable
typedef std::unordered_map<std::string, symindex> _IdentMap;
typedef std::vector<Symbol> _Symbols;

/*
Stores symbols and their values
*/
class SymTable {
	_IdentMap _varToSym; //identifier -> symindex
	_Symbols _table; // symIndex -> value

	int _symCount; //internal counter

public:
	//when the identifier has not been declared
	static const symindex NO_SYM = -1;

	SymTable() : _varToSym(), _table(), _symCount(0) {}

	inline symindex getSym(std::string *var) {
		_IdentMap::iterator x = _varToSym.find(*var);
		if (x != _varToSym.end()) { //already seen variable
			return (*x).second;
		}
		else return NO_SYM;
	}

	inline Symbol getSym(symindex sym) {
		assert(sym >= 0);
		assert(sym <= symNum());
		return _table[sym];
	}
	
	inline symindex addSym(std::string *var, EType t) {
			_varToSym.insert(std::pair<std::string, symindex>(std::string(*var), _symCount));
			_table.push_back( Symbol( t ) );
			_symCount++;
			return symNum();
	}

	inline void setSym(symindex sym, SymVal val) {
		_table[sym].val = val;
	}

	//number of symbols defined
	inline int symNum() {
		return _symCount - 1;
	}


};