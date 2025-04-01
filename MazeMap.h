//MazeMap.h
#ifndef MAZEMAP_H
#define MAZEMAP_H

#include <vector>
#include <string>

using namespace std;

struct coordinate{
    int y = 0;
    int x = 0;
};

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
