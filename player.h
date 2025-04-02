//player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>

struct coordinate{
    int y = 0;
    int x = 0;
};

using namespace std;

extern int player_heart; //玩家血量
extern int number_of_mineSweeping; //玩家有限的扫雷次数
extern vector <string> player_inventory; //特殊道具的储存空间
extern int number_of_key; //玩家获取的钥匙数量
extern coordinate Player_coordinate; //玩家的坐标


#endif
