#pragma once
#include <inttypes.h>
#include <vector>
#include <array>
#include "WorldBlock.h"
#include "Helper.h"
#include "Gameplay.h"
#include <utility>
#include <algorithm>
#include <unordered_map>

class World {
public:
	using BLOCK_CONSTRUCTOR = std::function<WorldBlock(const Position& pos)>;
public:
	const uint16_t WIDTH;
	const uint16_t HEIGHT;
protected:
	std::vector<WorldBlock> blocks;

	using WorldBlockProbability = std::pair<const WorldBlock*, float>;
	using WorldBlockProbabilityVector = std::pair<uint16_t, std::vector<WorldBlockProbability> >;
public : /** DEBUG */
	std::vector<WorldBlockProbabilityVector> movement_possiblity_map;
private:
	WorldBlock block_above_world_top = WorldBlock::create_block({ 0,-1 }, { 0.0f,0.0f }, gameplay::BlockType::HEAVEN_STONE, 0, 0);
	WorldBlock block_below_world_bottom = WorldBlock::create_block({ 0,(int16_t)HEIGHT }, { 0.0f,0.0f }, gameplay::BlockType::HELL_STONE, 10, 100000.0f);

public:
	World(const World& cpy) :
		WIDTH(cpy.WIDTH),
		HEIGHT(cpy.HEIGHT),
		blocks(cpy.blocks),
		movement_possiblity_map(cpy.movement_possiblity_map) {
	}

	World(const uint16_t width, const uint16_t height) :
		WIDTH(width),
		HEIGHT(height),
		blocks(size_t(width)* size_t(height)),
		movement_possiblity_map(size_t(width)* size_t(height)) {
		uint16_t blocks_count = width * height;
		blocks.reserve(blocks_count);
		std::fill(blocks.begin(), blocks.end(), WorldBlock());
		movement_possiblity_map.reserve(blocks_count);
		std::fill(movement_possiblity_map.begin(), movement_possiblity_map.end(), WorldBlockProbabilityVector());
		reset();
	}

	World& operator=(const World& other) {
		if (other.WIDTH == WIDTH && other.HEIGHT == HEIGHT) {
			blocks = other.blocks;
			movement_possiblity_map = other.movement_possiblity_map;
			return *this;
		}
		else {
			throw std::exception("incompatible");
		}
	}

	World& reset(BLOCK_CONSTRUCTOR block_constructor = WorldBlock::create_random_block) {
		const size_t blocks_size = blocks.size();
		for (size_t i = 0; i < blocks_size; ++i) {
			int16_t x = int16_t(i % WIDTH);
			int16_t y = int16_t(i / HEIGHT);
			blocks.at(i) = block_constructor({ x, y });
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
		return DirectNeighbours({ &at(pos.left()) , &at(pos.right()) , &at(pos.up()) , &at(pos.down()) });
	}

	World update() const {
		const World& current = *this;
		World result = World(WIDTH, HEIGHT);
		for (size_t i = 0; i < blocks.size(); ++i) {
			auto& block = blocks[i];
			auto neighbours = neighbours_at(block.position);
			WorldBlock result_block = block.get_update_result(neighbours);
			result.at(block.position) = result_block;
			result.movement_possiblity_map[i] = get_movement_possibilities(block, neighbours);
		}

		auto begin = result.movement_possiblity_map.begin();
		auto end = result.movement_possiblity_map.end();
		const auto compare_probability = [](const WorldBlockProbability& a, const WorldBlockProbability& b) {
			return a.second < b.second;
		};

		std::sort(begin, end, [compare_probability](const WorldBlockProbabilityVector& a, const WorldBlockProbabilityVector& b) -> bool {
			auto highest_a = std::max_element(a.second.begin(), a.second.end(), compare_probability);
			auto highest_b = std::max_element(b.second.begin(), b.second.end(), compare_probability);
			if (highest_a == a.second.end()) return false;
			if (highest_b == b.second.end()) return true;
			float a_probability = highest_a->second;
			float b_probability = highest_b->second;
			bool is_a_before_b = a_probability > b_probability;
			return is_a_before_b;
			});

		std::unordered_map<uint16_t, bool> moved_blocks;
		moved_blocks.reserve(uint16_t(WIDTH ) * HEIGHT);
		for (auto probability_vector : result.movement_possiblity_map) {
			uint16_t target_block_id = probability_vector.first;
			if (moved_blocks.find(target_block_id) == moved_blocks.end()) {
				// not yet moved target
				auto &most_likely_block = probability_vector.second.front();
				auto source_block_id = get_block_id(*most_likely_block.first);
				if (source_block_id != target_block_id ) {
					// not the same block
					if (moved_blocks.find(source_block_id) == moved_blocks.end()) {
						// not yet moved source
						auto& source_block = result.blocks.at(target_block_id);
						auto& target_block = result.blocks.at(source_block_id);

						// swapp blocks
						
						WorldBlock swap_buffer = target_block;
						target_block.set_to( source_block );
						source_block.set_to( swap_buffer );
						
						// reduce impulses
						auto direction = (target_block.position - source_block.position).get_as_type<float>();
						auto force_needed = (target_block.get_effective_mass() + source_block.get_effective_mass()) * 100.0f;
						source_block.impuls = source_block.impuls - direction * force_needed;
						target_block.impuls = target_block.impuls + direction * force_needed;

						// mark as moved
						moved_blocks.emplace(source_block_id, true);
						moved_blocks.emplace(target_block_id, true);
					}
				}
			}
		}
		return result;
	}



	std::vector<WorldBlock>::iterator begin() { return blocks.begin(); }
	std::vector<WorldBlock>::iterator end() { return blocks.end(); }

private:
	WorldBlockProbabilityVector get_movement_possibilities(const WorldBlock& block, const DirectNeighbours& neighbours) const {
		auto block_id = get_block_id(block);
		auto force_needed = (block.get_effective_mass() + block.get_effective_mass()) * 100.0f + 1.0f;
		WorldBlockProbabilityVector buffer(block_id, 0);
		WorldBlockProbability center_block_probabilty_pair = std::make_pair(&block, 1.0f - std::min(block.impuls.get_length2() / force_needed, 1.0f) );
		WorldBlockProbability top_block_probabilty_pair = std::make_pair(&neighbours.get_top(), get_probability_block_moves_to_position(neighbours.get_top(), block));
		WorldBlockProbability bottom_block_probabilty_pair = std::make_pair(&neighbours.get_bottom(), get_probability_block_moves_to_position(neighbours.get_bottom(), block));
		WorldBlockProbability left_block_probabilty_pair = std::make_pair(&neighbours.get_left(), get_probability_block_moves_to_position(neighbours.get_left(), block));
		WorldBlockProbability right_block_probabilty_pair = std::make_pair(&neighbours.get_right(), get_probability_block_moves_to_position(neighbours.get_right(), block));

		buffer.second.emplace_back(center_block_probabilty_pair);
		if (neighbours.get_top().is_immovable_block() == false) buffer.second.emplace_back(top_block_probabilty_pair);
		if (neighbours.get_bottom().is_immovable_block() == false) buffer.second.emplace_back(bottom_block_probabilty_pair);
		if (neighbours.get_left().is_immovable_block() == false) buffer.second.emplace_back(left_block_probabilty_pair);
		if (neighbours.get_right().is_immovable_block() == false) buffer.second.emplace_back(right_block_probabilty_pair);
		std::sort(buffer.second.begin(), buffer.second.end(), [](const WorldBlockProbability &a, const WorldBlockProbability &b)->bool {
			return a.second > b.second;
			});
		return buffer;
	}

	float get_probability_block_moves_to_position( const WorldBlock &source_block, const WorldBlock &target_block)const {
		auto force_needed = (target_block.get_effective_mass() + source_block.get_effective_mass()) * 100.0f;
		if (force_needed <= 0.0f) return 0.0f;
		auto direction = (target_block.position - source_block.position).get_normalized().get_as_type<float>();
		float impuls_in_direction = direction.get_scalar_product(source_block.impuls-target_block.impuls);
		float result = float( 0.5f + atanf(impuls_in_direction / force_needed) / M_PI);
		return result;
	}

	inline uint16_t get_block_id(const WorldBlock& block) const {
		return block.position.get_x() + block.position.get_y() * WIDTH;
	}
};
