//MazeMap.h
#ifndef ELEMENT_H
#define ELEMENT_H

#include <random>
#include <vector>

using namespace std;

extern random_device rd;
extern mt19937 gen;

struct coordinate{
    int y = 0;
    int x = 0;
};

void add_door_for_small_game (vector <vector <char>>& map1, vector <coordinate> Sihutong, int x, int y);

void add_mine (vector <vector <char>>& map1, vector<coordinate> changeFinish);

#endif
