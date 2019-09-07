#pragma once
#define _USE_MATH_DEFINES
#include <array>
#include <vector>
#include <algorithm>
#include "olcPixelGameEngine.h"
#include "Gameplay.h"
#include "World.h"
#include <math.h>

using namespace gameplay;



class GameEngine :
	public olc::PixelGameEngine
{
private:
	float game_time;
	float time_to_next_update;
	olc::Sprite* human_stand;
	World world = World(50u, 50u);

public:
	GameEngine()
	{
		sAppName = "Example";
		time_to_next_update = 0;
	}

public:
	bool OnUserCreate() override
	{
		auto color_skin = olc::Pixel(0xff69ace0);
		auto color_clothes = olc::Pixel(0xffdd6666);
		// Called once at the start, so create things here
		human_stand = new olc::Sprite(5, 6);
		human_stand->SetPixel(2, 1, color_skin);
		human_stand->SetPixel(2, 2, color_clothes);
		human_stand->SetPixel(1, 2, color_skin);
		human_stand->SetPixel(3, 2, color_skin);
		human_stand->SetPixel(2, 3, color_clothes);
		human_stand->SetPixel(2, 4, color_clothes);
		human_stand->SetPixel(1, 5, color_skin);
		human_stand->SetPixel(3, 5, color_skin);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		game_time += fElapsedTime;
		time_to_next_update -= fElapsedTime;
		// called once per frame

		if (time_to_next_update < 0) {
			world = world.update();
			time_to_next_update = 0.125;
		}


		for (auto world_block : world) {
			auto x = world_block.position.get_x();
			auto y = world_block.position.get_y();
			olc::Pixel color = this->get_pixel_for_block_in_render_mode(world_block);
			Draw(x, y, color);
		}


		//DrawSprite(5, 5, human_stand, 2);

		return true;

	}

	olc::Pixel get_pixel_for_block_in_render_mode(WorldBlock block) {
		if (this->GetKey(olc::Key::P).bHeld) {
			uint8_t value = uint8_t(std::min(long(block.pressure * 16), 255l));
			return olc::Pixel(value / 8, value / 8, value);
		}
		else if (this->GetKey(olc::Key::M).bHeld) {
			uint8_t value = uint8_t(std::min(long(block.mass), 255l));
			return olc::Pixel(value / 8, value, value / 8);
		}
		else if (this->GetKey(olc::Key::H).bHeld) {
			uint8_t value = uint8_t(std::min(long(block.heat), 255l));
			return olc::Pixel(value, value / 8, value / 8);
		}
		else if (this->GetKey(olc::Key::I).bHeld) {
			MyVector<float> normalized_impuls = block.impuls.get_normalized();
			uint8_t value_r = uint8_t(std::min(128 + uint8_t(127.0f * normalized_impuls.get_x()), 255));
			uint8_t value_b = uint8_t(std::min(128 + uint8_t(127.0f * normalized_impuls.get_y()), 255));
			return olc::Pixel(value_r, 0, value_b);
		}
		return get_color_of_block_type(block.type);
	}

};

