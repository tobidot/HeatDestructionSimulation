#pragma once
#include <inttypes.h>
#include "Gameplay.h"
#include "AttributeTypes.h"
#include "Helper.h"

struct WorldBlock {
	Position position = { 0,0 };
	gameplay::BlockType type = gameplay::BlockType::HELL_STONE;
	Pressure pressure = 1;
	Mass mass = 1;


public:
	WorldBlock& get_resulting_impuls(const DirectNeighbours &neighbours) {
		return *this;
	}

	bool is_of_type(gameplay::BlockType type) {
		return this->type == type;
	}
	
	bool is_out_of_map() {
		return position.get_y() < 0 || position.get_x() < 0;
	}

	static WorldBlock create_block_of_type(int16_t x, int16_t y, gameplay::BlockType type) {
		return WorldBlock({ Position({x, y}), type, 1, gameplay::get_mass_of_block_type(type) });
	}

	static inline WorldBlock create_random_block(int16_t x, int16_t y) {
		gameplay::BlockType type = gameplay::BlockType(rand() % gameplay::BlockType::MAX);
		return WorldBlock::create_block_of_type(x, y, type);
	}
};

