//MazeMap.h
#ifndef MAZEMAP_H
#define MAZEMAP_H

#include <vector>
#include <string>
#include "player.h"

using namespace std;

struct Maps{
    vector<vector<char>> MazeMap_hide;
    vector<vector<char>> MazeMap_show;
};

bool breakWall(int y, int x, int Cy, int Cx, vector<vector<char>>& map1, vector<coordinate>& changeFinish);

coordinate returnTolast(vector<coordinate> changeFinish, int times);

coordinate choose_start (int y);

coordinate choose_end (int y, int x);

Maps Make_Map(int x, int y);

#endif
