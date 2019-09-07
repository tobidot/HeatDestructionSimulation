#pragma once
#include <inttypes.h>
#include <functional>
#include <numeric>
#include "Gameplay.h"
#include "AttributeTypes.h"
#include "Helper.h"

struct WorldBlock {
	Position position = { 0,0 };
	Position impuls = { 0,0 };
	gameplay::BlockType type = gameplay::BlockType::HELL_STONE;
	Pressure pressure = 1;
	Mass mass = 1;
	Heat heat = 1000.0f;


public:
	WorldBlock get_update_result(const DirectNeighbours& neighbours) {
		WorldBlock block = *this;		
		block.heat = get_updated_heat(neighbours);
		block.impuls = get_updated_impuls(neighbours);
		return block;
	}

	bool is_of_type(gameplay::BlockType type) {
		return this->type == type;
	}

	bool is_out_of_map() {
		return position.get_y() < 0 || position.get_x() < 0;
	}

private:
	WorldBlock(const Position& pos, const Impuls &impuls, gameplay::BlockType type, Pressure pressure, Heat heat) {
		this->position = pos;
		this->type = type;
		this->pressure = pressure;
		this->heat = heat;
		this->impuls = impuls;
		this->mass = get_mass_of_block_type(type);
	}
	Impuls get_updated_impuls(const DirectNeighbours& neighbours) const {
		return { 0,0 };
	}
	Heat get_updated_heat(const DirectNeighbours& neighbours) const {
		const WorldBlock &block = *this;
		auto extract_heat_from_block = [](const WorldBlock* block) {return block->heat; };
		auto heat_accumulator = [](Heat acc, const WorldBlock* block) {return acc + block->heat; };
		auto sum_heat = std::accumulate(neighbours.begin(), neighbours.end(), 0, heat_accumulator);
		auto medium_heat = (sum_heat + block.heat) / 5;
		auto heat_diff = (atanf(medium_heat - block.heat) + 1.0f) / 2.0f;

		float heat_cooeficient = heat_diff * heat_diff / (block.mass + 1.0f);
		float heat_result = Heat(medium_heat * heat_cooeficient + block.heat * (1 - heat_cooeficient));
		return heat_result;
	}

public:
	static WorldBlock create_block(const Position& pos, const Impuls& impulse, gameplay::BlockType type, Pressure pressure, Heat heat) {
		return WorldBlock( pos, impulse, type, pressure, heat );
	}

	static WorldBlock create_block_of_type(const Position& pos, gameplay::BlockType type) {
		return WorldBlock::create_block(pos, { 0,0 }, type, 1, 10);
	}

	static inline WorldBlock create_random_block(const Position& pos) {
		gameplay::BlockType type = gameplay::BlockType(rand() % gameplay::BlockType::MAX);
		return WorldBlock::create_block(pos, {0,0 }, type, rand() % 10, rand() % 100);
	}
};

