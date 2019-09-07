#define OLC_PGE_APPLICATION
#define _USE_MATH_DEFINES
#include "GameEngine.h"

int main(char** args, int argc) {
	GameEngine demo;
	if (demo.Construct(50, 50, 16, 16, false)) {
		demo.Start();
	}

	return 0;
}