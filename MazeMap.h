//MazeMap.h
#ifndef MAZEMAP_H
#define MAZEMAP_H

#include <vector>

using namespace std;

struct coordinate{
    int y = 0;
    int x = 0;
};

bool breakWall(int y, int x, int Cy, int Cx, vector<vector<char>>& map1, vector<coordinate>& changeFinish);

coordinate returnTolast(vector<coordinate> changeFinish, int times);

coordinate choose_start (int y);

coordinate choose_end (int y, int x);

vector<vector<char>> Make_Map(int x, int y);

#endif
