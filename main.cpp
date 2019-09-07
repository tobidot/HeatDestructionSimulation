#define OLC_PGE_APPLICATION
#include "GameEngine.h"

int main(char** args, int argc) {
	GameEngine demo;
	if (demo.Construct(50, 50, 16, 16, false)) {
		demo.Start();
	}

	return 0;
}