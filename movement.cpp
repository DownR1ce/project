#include <string>
#include <iostream>
#include "player.h"
#include "MazeMap.h"

string player_movement (string order, Maps& maps){
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
    string tips;
    if (maps.MazeMap_show[maybe.y][maybe.x] != '#' || maps.MazeMap_show[maybe.y][maybe.x] != '@'){
        if (maps.MazeMap_hide[maybe.y][maybe.x] == '.' || maps.MazeMap_hide[maybe.y][maybe.x] == 'S'){
            maps.MazeMap_show [Player_coordinate.y][Player_coordinate.x] = '.';
            Player_coordinate = maybe;
            maps.MazeMap_show[maybe.y][maybe.x] = 'P';
        }
        if (maps.MazeMap_hide[maybe.y][maybe.x] == 'X'){
            tips = "你踩到了炸弹, -1生命值";
            maps.MazeMap_show [Player_coordinate.y][Player_coordinate.x] = '.';
            Player_coordinate = maybe;
            maps.MazeMap_show[maybe.y][maybe.x] = 'X';
            maps.MazeMap_hide[maybe.y][maybe.x] = '.'; // 踩到炸弹后，炸弹失效，炸掉一滴血。
            player_heart --;
        }
        if (maps.MazeMap_hide[maybe.y][maybe.x] == '?'){
            tips = "你找到了神秘的门, 按e进入";
            maps.MazeMap_show[maybe.y][maybe.x] = '?';
        }
        if (maps.MazeMap_hide[maybe.y][maybe.x] == 'D'){
            tips = "你找到了出口，但是钥匙呢？\n 摁e开锁";
            maps.MazeMap_show[maybe.y][maybe.x] = 'D';
        }
    }
    else{
        tips = "不能撞墙";
    }
    return tips;
}
