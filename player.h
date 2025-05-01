//player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>

struct coordinate{
    int y;
    int x;
};

using namespace std;

extern int player_heart; //玩家血量
extern int number_of_mineSweeping; //玩家有限的扫雷次数
extern vector <string> player_inventory; //特殊道具的储存空间
extern int number_of_key; //玩家获取的钥匙数量
extern coordinate Player_coordinate; //玩家的坐标
extern int XuYaoDe_number_of_key; // 需要通关的钥匙数量
extern int number_of_mine;
void getHeart();
extern vector <string> NameSaved;
void PrintSaved();


#endif
