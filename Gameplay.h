#pragma once
#include <inttypes.h>
#include "olcPixelGameEngine.h"


namespace gameplay {

	namespace consts {
		const float GRAVITY = 10.0f;
	};

	enum BlockType {
		VACUUM = 0,
		DIRT,
		AIR,
		HELL_STONE,
		HEAVEN_STONE,
		MAX
	};

	uint16_t get_mass_of_block_type(BlockType type);
	olc::Pixel get_color_of_block_type(BlockType type);

	
};