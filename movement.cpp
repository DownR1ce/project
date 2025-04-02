#include <string>
#include <iostream>
#include "player.h"
#include "MazeMap.h"

void player_movement (string order, Maps& maps){
    coordinate maybe;
    if (order == "w"){
        maybe = {Player_coordinate.y - 1 , Player_coordinate.x};
    }
    if (order == "s") {
        maybe = {Player_coordinate.y + 1 , Player_coordinate.x};
    }
    if (order == "a"){
        maybe = {Player_coordinate.y, Player_coordinate.x - 1};
    }
    if (order == "d"){
        maybe = {Player_coordinate.y, Player_coordinate.x + 1};
    }
    if (maps.MazeMap_show[maybe.y][maybe.x] != '#' || maps.MazeMap_show[maybe.y][maybe.x] != '@'){
        if (maps.MazeMap_hide[maybe.y][maybe.x] == '.' || maps.MazeMap_hide[maybe.y][maybe.x] == 'S'){
            maps.MazeMap_show [Player_coordinate.y][Player_coordinate.x] = '.';
            Player_coordinate = maybe;
            maps.MazeMap_show[maybe.y][maybe.x] = 'P';
        }
        if (maps.MazeMap_hide[maybe.y][maybe.x] == 'X'){
            cout<< "你踩到了炸弹" << endl;
            maps.MazeMap_show [Player_coordinate.y][Player_coordinate.x] = '.';
            Player_coordinate = maybe;
            maps.MazeMap_show[maybe.y][maybe.x] = 'X';
        }
        if (maps.MazeMap_hide[maybe.y][maybe.x] == '?'){
            cout << "你找到了神秘的门, 按e进入" << endl;
            maps.MazeMap_show[maybe.y][maybe.x] = '?';
        }
        if (maps.MazeMap_hide[maybe.y][maybe.x] == 'D'){
            cout << "你找到了出口，但是钥匙呢？\n 摁e开锁" << endl;
            maps.MazeMap_show[maybe.y][maybe.x] = 'D';
        }
    }
    if (maps.MazeMap_show[maybe.y][maybe.x] == '#' || maps.MazeMap_show[maybe.y][maybe.x] == '@'){
        cout << "不能撞墙" << endl;
    }
    return;
}
