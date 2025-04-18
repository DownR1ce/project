#include <iostream>
#include "player.h"

using namespace std;

//这个文件到时候用来装小游戏用，通关获得钥匙的那个小游戏

void JiaZhuangTaShiYiGeYouXi (){
    cout << "这真的是一个游戏，你获得了一个钥匙" << endl;
    cout << "输入一个zimu出去" << endl;
    char a;
    cin >> a;
    number_of_key ++;
    return; 
}
