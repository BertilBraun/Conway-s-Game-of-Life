#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <random>
#include <iostream>

int w = 40;
int h = 40;
float ttu = 0.2;

class Game : public olc::PixelGameEngine
{
public:
	bool* map = nullptr;

	Game()
	{
		sAppName = "Game of Life";
	}

	bool OnUserCreate()
	{
		createRandomMap();

		return true;
	}

	void createRandomMap() {
		if (map)
			delete map;

		map = new bool[w * h];

		for (int x = 0; x < w; x++)
			for (int y = 0; y < h; y++)
				map[Lin(x, y)] = (rand() % 3 == 0 ? 1 : 0);
	}

	void createClearMap() {
		if (map)
			delete map;

		map = new bool[w * h]{ false };
	}

	bool simulating = true;

	bool OnUserUpdate(float fElapsedTime)
	{
		if (GetKey(olc::SPACE).bPressed)
			simulating = !simulating;

		if (simulating)
			updateMap(fElapsedTime);

		if (GetKey(olc::C).bPressed)
			createClearMap();

		if (GetKey(olc::R).bPressed)
			createRandomMap();

		if (GetMouse(0).bPressed || GetMouse(0).bHeld) {
			simulating = false;
			map[Lin(GetMouseX(), GetMouseY())] = true;
		}
		if (GetMouse(1).bPressed || GetMouse(1).bHeld)
			map[Lin(GetMouseX(), GetMouseY())] = false;

		drawMap();
		if (simulating)
			Draw(0, 0, olc::RED);
		return true;
	}

	float timeSinceLastUpdate = 0;

	void updateMap(float dt) {
		timeSinceLastUpdate += dt;
		if (timeSinceLastUpdate < ttu)
			return;
		timeSinceLastUpdate -= ttu;

		bool* newMap = new bool[w * w];

		for (int x = 0; x < w; x++)
			for (int y = 0; y < h; y++) {
				int aliveNeighbours = getAliveNeighbours(x, y);

				int idx = Lin(x, y);
				if (map[idx] == true && (aliveNeighbours == 2 || aliveNeighbours == 3))
					newMap[idx] = true;
				else if (map[idx] == false && aliveNeighbours == 3)
					newMap[idx] = true;
				else
					newMap[idx] = false;
			}

		delete map;
		map = newMap;
	}

	void drawMap() {
		for (int x = 0; x < w; x++)
			for (int y = 0; y < h; y++)
				Draw(x, y, map[Lin(x, y)] ? olc::WHITE : olc::BLACK);
	}

	int getAliveNeighbours(int x, int y) {
		int count = 0;

		for (int xi = -1; xi <= 1; xi++)
			for (int yi = -1; yi <= 1; yi++)
				if (xi != 0 || yi != 0)
					if (xi + x >= 0 && xi + x < w && yi + y >= 0 && yi + y < w)
						if (map[Lin(xi + x, yi + y)] == true)
							count++;
		return count;
	}

	int Lin(int x, int y) {
		return x + w * y;
	}
};


int main()
{

	Game game;
	if (game.Construct(w, h, 20, 20))
		game.Start();

	return 0;
};

