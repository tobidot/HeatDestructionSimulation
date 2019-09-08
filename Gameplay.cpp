#include "Gameplay.h"
#include "olcPixelGameEngine.h"

namespace gameplay {
	olc::Pixel get_color_of_block_type(BlockType type) {
		switch (type) {
		case VACUUM:
			return olc::Pixel(0xff000000);
		case DIRT:
			return olc::Pixel(0xff446688);
		case AIR:
			return olc::Pixel(0xffee9999);
		case HEAVEN_STONE:
			return olc::Pixel(0xffffdddd);
		case HELL_STONE:
			return olc::Pixel(0xff042233);
		default:
			return olc::Pixel(0xff888888);
		};
	}

	Mass get_mass_of_block_type(BlockType type) {
		switch (type) {
		case VACUUM:
			return 0;
		case DIRT:
			return 100;
		case AIR:
			return 1;
		case HEAVEN_STONE:
			return 0;
		case HELL_STONE:
			return 100;
		default:
			return 1;
		}
	}
}