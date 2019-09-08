#pragma once
#include <inttypes.h>
#include "olcPixelGameEngine.h"
#include "AttributeTypes.h"


namespace gameplay {

	namespace consts {
		const float GRAVITY = 0.05f;
		const float IMPULS_TRANSMITION_COEFFICIENT = 0.1f;
		const float PRESSURE_FORCE_COEFFICIENT = 0.1f;
	};

	enum BlockType {
		VACUUM = 0,
		DIRT,
		AIR,
		HELL_STONE,
		HEAVEN_STONE,
		MAX
	};

	Mass get_mass_of_block_type(BlockType type);
	olc::Pixel get_color_of_block_type(BlockType type);
	BlockType get_random_block_type();


};