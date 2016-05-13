#pragma once

#include <string>
#include <algorithm>

inline void toLower(std::string *str) {
	std::transform(str->begin(), str->end(), str->begin(), ::tolower);
}

inline bool isNum(char x) {
	return x >= '0' && x <= '9';
}

inline bool isAlpha(char x) {
	return x >= 'A' && x <= 'z' && x != '[' && x != ']' && x != '^' && x != '_' && x != '`';
}

inline bool isWhitespace(char x) {
	return  (x == ' ' || x == '\n' || x == '\t');
}