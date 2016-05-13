#pragma once
#include <iostream>

//struct that represents a position on the file

struct filepos {
	int line, col;
	filepos() : line(-1), col(-1) {}
	filepos(int l, int p) :line(l), col(p) {
	}
};

inline std::ostream& operator<<(std::ostream &stream, const filepos &pos) {
	stream << "line " << pos.line << ", col " << pos.col;
	return stream;
};