#pragma once
#include <inttypes.h>
#include <vector>
#include <array>
#include "WorldBlock.h"
#include "Helper.h"
#include "Gameplay.h"

class World {
public:
	using BLOCK_CONSTRUCTOR = std::function<WorldBlock(uint16_t x, uint16_t y)>;
public:
	const uint16_t WIDTH;
	const uint16_t HEIGHT;
protected:
	std::vector<WorldBlock> blocks;
private:
	WorldBlock block_above_world_top = WorldBlock::create_block_of_type(0,-1,gameplay::BlockType::HEAVEN_STONE);
	WorldBlock block_below_world_bottom = WorldBlock::create_block_of_type(0,HEIGHT, gameplay::BlockType::HELL_STONE);

public:
	World(const World& cpy) :
		WIDTH(cpy.WIDTH),
		HEIGHT(cpy.HEIGHT),
		blocks(cpy.blocks) {
	}

	World(const uint16_t width, const uint16_t height) :
		WIDTH(width),
		HEIGHT(height),
		blocks(width* height) {
		size_t blocks_count = width * height;
		blocks.reserve(width * height);
		std::fill(blocks.begin(), blocks.end(), WorldBlock());
		size_t i = 0;
		reset();
	}

	World& operator=(const World& other) {
		if (other.WIDTH == WIDTH && other.HEIGHT == HEIGHT) {
			blocks = other.blocks;
			return *this;
		}
		else {
			throw std::exception("incompatible");
		}
	}

	World& reset(BLOCK_CONSTRUCTOR block_constructor = WorldBlock::create_random_block) {
		const size_t blocks_size = blocks.size();
		for (size_t i = 0; i < blocks_size; ++i) {
			int16_t x = int16_t( i % WIDTH );
			int16_t y = int16_t( i / HEIGHT );
			blocks.at(i) = block_constructor(x, y);
		}
		return *this;
	}

	const WorldBlock& at(Position position) const {
		int16_t x = position.get_x();
		int16_t y = position.get_y();

		if (y >= HEIGHT) return block_below_world_bottom;
		if (y < 0) return block_above_world_top;
		while (x >= WIDTH) x -= WIDTH;
		while (x < 0) x += WIDTH;
		return blocks.at(WIDTH * y + x);
	}

	WorldBlock& at(const Position position) {
		return const_cast<WorldBlock&>(const_cast<const World*>(this)->at(position));
	}

	DirectNeighbours neighbours_at(const Position pos) const {
		return DirectNeighbours({ &at(pos.left()) , &at(pos.right()) , &at(pos.top()) , &at(pos.bottom()) });
	}

	World update() const {
		const World& current = *this;
		World result = World(WIDTH, HEIGHT);
		for (auto block : current.blocks) {
			auto result_block = block.get_resulting_impuls(neighbours_at(block.position));
			result.at(block.position) = result_block;
		}
		return result;
	}

	std::vector<WorldBlock>::iterator begin() { return blocks.begin(); }
	std::vector<WorldBlock>::iterator end() { return blocks.end(); }

};
