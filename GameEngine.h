#pragma once
#include <array>
#include <vector>
#include "olcPixelGameEngine.h"
#include "Gameplay.h"
#include "World.h"

using namespace gameplay;



class GameEngine :
	public olc::PixelGameEngine
{
private:
	float game_time;
	float time_to_next_update;
	olc::Sprite* human_stand;
	World world = World(64u, 64u);

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
			auto color = get_color_of_block_type(world_block.type);
			Draw(x, y, color);
		}


		DrawSprite(5, 5, human_stand, 2);

		return true;
	}
};

