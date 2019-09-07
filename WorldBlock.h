#pragma once
#include <inttypes.h>
#include <functional>
#include <algorithm>
#include <numeric>
#include "Gameplay.h"
#include "AttributeTypes.h"
#include "Helper.h"

struct WorldBlock {
	Position position = { 0,0 };
	Impuls impuls = { 0,0 };
	gameplay::BlockType type = gameplay::BlockType::HELL_STONE;
	Pressure pressure = 1;
	Mass mass = 1;
	Heat heat = 1000.0f;


public:
	WorldBlock() {

	}
	WorldBlock(const Position& pos, const Impuls& impuls, gameplay::BlockType type, Pressure pressure, Heat heat) {
		this->position = pos;
		this->type = type;
		this->pressure = pressure;
		this->heat = heat;
		this->impuls = impuls;
		this->mass = get_mass_of_block_type(type);
	}
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
	Impuls get_updated_impuls(const DirectNeighbours& neighbours) const {
		const WorldBlock& block = *this;
		auto effective_mass = block.pressure * block.mass;
		auto gravity_impuls = Impuls(0.0f, effective_mass * gameplay::consts::GRAVITY);

		auto horizontal_pressure_diff = neighbours.get_right().pressure - neighbours.get_left().pressure;
		auto vertical_pressure_diff = neighbours.get_bottom().pressure - neighbours.get_top().pressure;
		auto pressure_impuls = Impuls(horizontal_pressure_diff / 1.0f, vertical_pressure_diff / 1.0f);

		auto impuls_from_top = std::max(neighbours.get_top().impuls.get_y(), 0.0f) / 10.f;
		auto impuls_from_bottom = std::min(neighbours.get_bottom().impuls.get_y(), 0.0f) / 10.f;
		auto impuls_from_left = std::max(neighbours.get_left().impuls.get_x(), 0.0f) / 10.f;
		auto impuls_from_right = std::min(neighbours.get_right().impuls.get_x(), 0.0f) / 10.f;
		auto neighbour_impuls = Impuls(impuls_from_top + impuls_from_bottom, impuls_from_left + impuls_from_right);

		return block.impuls + gravity_impuls + pressure_impuls + neighbour_impuls;
	}
	Heat get_updated_heat(const DirectNeighbours& neighbours) const {
		const WorldBlock& block = *this;
		auto extract_heat_from_block = [](const WorldBlock* block) {return block->heat; };
		auto heat_accumulator = [](Heat acc, const WorldBlock* block) {return acc + block->heat; };
		auto sum_heat = std::accumulate(neighbours.begin(), neighbours.end(), Heat(0), heat_accumulator);
		auto medium_heat = (sum_heat + block.heat) / 5;
		auto heat_diff = (atanf(medium_heat - block.heat) + 1.0f) / 2.0f;

		float heat_cooeficient = heat_diff * heat_diff / (block.mass + 1.0f);
		float heat_result = Heat(medium_heat * heat_cooeficient + block.heat * (1 - heat_cooeficient));
		return heat_result;
	}

public:
	static WorldBlock create_block(const Position& pos, const Impuls& impulse, gameplay::BlockType type, Pressure pressure, Heat heat) {
		return WorldBlock(pos, impulse, type, pressure, heat);
	}

	static WorldBlock create_block_of_type(const Position& pos, gameplay::BlockType type) {
		return WorldBlock::create_block(pos, { 0.0f,0.0f }, type, 1, 10);
	}

	static inline WorldBlock create_random_block(const Position& pos) {
		gameplay::BlockType type = gameplay::BlockType(rand() % gameplay::BlockType::MAX);
		return WorldBlock::create_block(pos, { 0.0f,0.0f }, type, rand() % 10, Heat(rand() % 100));
	}
};

