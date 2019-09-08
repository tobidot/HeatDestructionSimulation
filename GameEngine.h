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
	float time_to_next_camera_update;
	olc::Sprite* human_stand;
	World world = World(50u, 50u);
	Position camera = Position(25, 25);

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
		time_to_next_camera_update -= fElapsedTime;
		// called once per frame

		if (time_to_next_update < 0) {
			world = world.update();
			time_to_next_update = 0.125;
		}
		if (time_to_next_camera_update < 0) {
			update_camera_input();
			time_to_next_camera_update = 0.015625;
		}


		for (int16_t x = 0; x < this->GetDrawTargetWidth(); ++x) {
			for (int16_t y = 0; y < this->GetDrawTargetHeight(); ++y) {
				auto world_x = x + camera.get_x() - world.WIDTH / 2;
				auto world_y = y + camera.get_y() - world.HEIGHT / 2;
				auto block = world.at(Position(world_x, world_y));
				olc::Pixel color = this->get_pixel_for_block_in_render_mode(block);
				Draw(x, y, color);
			}
		}

		return true;

	}

	void update_camera_input() {
		if (this->GetKey(olc::Key::LEFT).bHeld) {
			camera = camera.left();
			if (camera.get_x() < 0) camera.set_x(world.WIDTH-1);
		}
		if (this->GetKey(olc::Key::RIGHT).bHeld) {
			camera = camera.right();
			if (camera.get_x() >= world.WIDTH) camera.set_x(0);
		}
		if (this->GetKey(olc::Key::UP).bHeld) {
			camera = camera.up();
			if (camera.get_y() < 0) camera.set_y(0);
		}
		if (this->GetKey(olc::Key::DOWN).bHeld) {
			camera = camera.down();
			if (camera.get_y() > world.HEIGHT) camera.set_y(world.HEIGHT);
		}
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
			if (abs(block.impuls.get_x()) < 0.001 && abs(block.impuls.get_y() < 0.001)) return olc::Pixel(0, 0, 0);
			MyVector<float> normalized_impuls = block.impuls.get_normalized();
			uint8_t value_r = uint8_t(std::min(128 + int8_t(127.0f * normalized_impuls.get_x()), 255));
			uint8_t value_b = uint8_t(std::min(128 + int8_t(127.0f * normalized_impuls.get_y()), 255));
			return olc::Pixel(value_r, 0, value_b);
		}
		return get_color_of_block_type(block.type);
	}

};

