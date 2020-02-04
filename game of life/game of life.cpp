#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <random>
#include <iostream>

const int w = 200;

class Game : public olc::PixelGameEngine
{
public:
	bool* map;

	Game()
	{
		sAppName = "Game of Life";
	}

	bool OnUserCreate()
	{
		map = new bool[w * w];

		for (int x = 0; x < w; x++)
			for (int y = 0; y < w; y++)
				map[x + w * y] = (rand() % 3 == 0 ? 1 : 0);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		bool* newMap = new bool[w * w];

		for (int x = 0; x < w; x++)
			for (int y = 0; y < w; y++) {
				int aliveNeighbours = getAliveNeighbours(x, y);

				if (map[x + w * y] == true && (aliveNeighbours == 2 || aliveNeighbours == 3))
					newMap[x + w * y] = true;
				else if (map[x + w * y] == false && aliveNeighbours == 3)
					newMap[x + w * y] = true;
				else
					newMap[x + w * y] = false;
			}

		delete map;
		map = newMap;

		for (int x = 0; x < w; x++)
			for (int y = 0; y < w; y++)
				Draw(x, y, map[x + w * y] ? olc::WHITE : olc::BLACK);

		Sleep(200);

		return true;
	}

	int getAliveNeighbours(int x, int y) {
		int count = 0;

		for (int xi = -1; xi <= 1; xi++)
			for (int yi = -1; yi <= 1; yi++)
				if (!(xi == 0 && yi == 0))
					if (xi + x >= 0 && xi + x < w && yi + y >= 0 && yi + y < w)
						if (map[(xi + x) + w * (yi + y)] == true)
							count++;

		return count;
	}

};


int main()
{

	Game game;
	if (game.Construct(320, 240, 4, 4))
		game.Start();

	return 0;
};

