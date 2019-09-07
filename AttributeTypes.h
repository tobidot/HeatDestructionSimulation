#pragma once
#include <inttypes.h>


struct Position : public std::pair<int16_t, int16_t> {
private:
	using SUPER = std::pair<int16_t, int16_t>;
public:
	using SUPER::SUPER;
	Position() : SUPER() {};
	Position(int16_t x, int16_t y) : SUPER(x,y) {};
	int16_t inline get_x() const { return this->first; }
	int16_t inline get_y() const { return this->second; }
	const Position left(int16_t amount = 1) const { return Position(first-amount,second); }
	const Position right(int16_t amount = 1) const { return Position(first+amount,second); }
	const Position top(int16_t amount = 1) const { return Position(first,second-amount); }
	const Position bottom(int16_t amount = 1) const { return Position(first,second+amount); }
};

struct Impuls {
	int16_t x;
	int16_t y;
};

using Pressure = int16_t;
using Mass = uint16_t;