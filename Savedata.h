//Savedata.h
#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <vector>
#include <string>
#include "player.h"

using namespace std;

struct GameSaveData {
    int player_heart;
    std::vector<std::string> player_inventory;
    int number_of_mineSweeping;
    int number_of_key;
    int XuYaoDe_number_of_key;
    coordinate Player_coordinate;
    int number_of_mine;
    int x;
    int y;
    char difficulty_of_the_quanbuyouxi;

    std::vector<std::vector<char>> MazeMap_hide;
    std::vector<std::vector<char>> MazeMap_show;
};

bool loadGame(GameSaveData& data, const string& filename);
void saveGame(const GameSaveData& data, const string& filename);
void savenameintofile(string name);
void showsavename(vector<string>& name);

#endif
