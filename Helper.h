#pragma once
#include <assert.h>
#include <functional>

struct WorldBlock;

class DirectNeighbours : public std::array<const WorldBlock*, 4> {
private:
	using SELF = DirectNeighbours;
	using SUPER = std::array<const WorldBlock*, 4>;
public:
	DirectNeighbours() = delete;
	DirectNeighbours(const SELF& cpy) : SUPER(cpy) {};
	DirectNeighbours(const SUPER& cpy) : SUPER(cpy) {
		assert(cpy.size() == 4 && std::none_of(
			cpy.begin(),
			cpy.end(),
			[](const auto* block) { return block == nullptr; }
		) && "All values must be set and cannot be null.");
	};
	const WorldBlock& get_left() const { return *this->at(0); }
	const WorldBlock& get_right() const { return *this->at(1); }
	const WorldBlock& get_top() const { return *this->at(2); }
	const WorldBlock& get_bottom() const { return *this->at(3); }
};