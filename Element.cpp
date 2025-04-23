//Element.cpp
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include "player.h"

using namespace std;

random_device rd;
mt19937 gen(rd());

void add_door_for_small_game (vector <vector <char>>& map1, vector <coordinate> Sihutong, int x, int y){
    vector<int> door;
    vector<vector<coordinate>> door_coordinate;
    uniform_int_distribution<> dis(0, Sihutong.size()-1);
    int doors = sqrt(x*y);
    doors = doors / 10;
    if (x >= 25 || y >= 25){
        doors ++;
    }
    XuYaoDe_number_of_key = doors;
    for (int i = 0; i<doors; i++){
        door.push_back(dis(gen));
    }
    for (int i = 0; i < door.size(); i++){
        map1[Sihutong[door[i]].y][Sihutong[door[i]].x] = '?';
    }
    return;
}

void add_mine (vector <vector <char>>& map1, vector <coordinate> changeFinish){
    int numberOfMine = changeFinish.size() / 6;
    number_of_mine = numberOfMine;
    uniform_int_distribution<> dis(0, changeFinish.size()-1);
    int a;
    vector <int> Mine_coordinate;
    while (true){  
        a = dis(gen);
        if ((find(Mine_coordinate.begin(), Mine_coordinate.end(), a) != Mine_coordinate.end()) || map1[changeFinish[a].y][changeFinish[a].x] != '.'){
            continue;
        }
        else{
            Mine_coordinate.push_back(a);
        }
        if (Mine_coordinate.size()==numberOfMine){
            break;
        }
    }
    for(int i = 0; i < numberOfMine; i++){
        map1[changeFinish[Mine_coordinate[i]].y][changeFinish[Mine_coordinate[i]].x] = 'X';//X用来表示雷
    }
    return;
}
