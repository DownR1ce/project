#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>
#include "Element.h"

using namespace std;

struct Maps{
    vector<vector<char>> MazeMap_hide;
    vector<vector<char>> MazeMap_show;
};

//输入初始点，然后打破墙，然后寻路
//第一个y和x是地图大小，cy和cx是坐标
bool breakWall(int y, int x, int Cy, int Cx, vector<vector<char>>& map1, vector<coordinate>& changeFinish){
    vector<coordinate> Possible;
    if (Cy-2>0 && (map1[Cy-2][Cx]=='p' || map1[Cy-2][Cx]=='D')){
        Possible.push_back({Cy-2, Cx});
    }
    if (Cy+2<=y-1 && (map1[Cy+2][Cx]=='p' || map1[Cy+2][Cx]=='D')){
        Possible.push_back({Cy+2, Cx});
    }
    if (Cx-2>0 && (map1[Cy][Cx-2]=='p' || map1[Cy][Cx-2]=='D')){
        Possible.push_back({Cy, Cx-2});
    }
    if (Cx+2<=x-1 && (map1[Cy][Cx+2]=='p' || map1[Cy][Cx+2]=='D')){
        Possible.push_back({Cy, Cx+2});
    }

    if (Possible.size() == 0){
        return false;
    }

    uniform_int_distribution<> dis(0, Possible.size()-1);
    int a = dis(gen);
    if (map1[Possible[a].y][Possible[a].x]!='D'){
        map1[Possible[a].y][Possible[a].x] = '.';
    }

    if(Possible[a].y == Cy){
        if((Cx-2)==Possible[a].x){
            map1[Cy][Cx-1] = '.';
            changeFinish.push_back({Cy,Cx-1});
            changeFinish.push_back(Possible[a]);
        }
        if((Cx+2)==Possible[a].x){
            map1[Cy][Cx+1] = '.';
            changeFinish.push_back({Cy,Cx+1});
            changeFinish.push_back(Possible[a]);
        }
    }
    if(Possible[a].x == Cx){
        if((Cy-2)==Possible[a].y){
            map1[Cy-1][Cx] = '.';
            changeFinish.push_back({Cy-1,Cx});
            changeFinish.push_back(Possible[a]);
        }
        if((Cy+2)==Possible[a].y){
            map1[Cy+1][Cx] = '.';
            changeFinish.push_back({Cy+1,Cx});
            changeFinish.push_back(Possible[a]);
        }
    }
    return true;
}

//如果没有可以打破的墙，则返回上一个起点
coordinate returnTolast(vector<coordinate> changeFinish, int times){
    if (changeFinish.size() < times*2) {
        cerr << "Error: Invalid access in returnTolast" << endl;
        return {-1, -1};  // 返回一个默认值或抛出异常
    }
    coordinate lastCoordinate = changeFinish[changeFinish.size()-(times*2-1)];
    return lastCoordinate;
}


coordinate choose_start (int y){
    int point = (y-1)/2;
    uniform_int_distribution<> dis(1,point);
    int start = dis(gen);
    start=start*2-1;
    return {start, 1};
}

coordinate choose_end (int y, int x){
    int point = (y-1)/2;
    uniform_int_distribution<> dis(1,point);
    int end = dis(gen);
    end = end*2-1;
    return {end, x-2};
}


//确保x和y都为奇数
Maps Make_Map(int x, int y){
    while (true){
        vector<char> hang;
        vector<vector<char>> map1;
        vector<char> hang0 (x, '#');
        vector<char> bian (x, '@');
        vector <vector <char>> Show_Map;
        for (int i = 0; i<x; i++){
            if(i%2==0){
                hang.push_back('#');  
            }
            else{
                hang.push_back('p');
            }
        }
        hang[0]='@';
        hang[x-1]='@';
        hang0[0]='@';
        hang0[x-1]='@';
        for (int i = 0; i<y; i++){
            if (i==0 || i==(y-1)){
                map1.push_back(bian);
                Show_Map.push_back(bian);
            }
            else if (i%2==0){
                map1.push_back(hang0);
                Show_Map.push_back(hang0);
            }
            else if (i%2==1){
                map1.push_back(hang);
                Show_Map.push_back(hang);
            }
        }
        
        //确定所有1的位置
        //存储位置坐标
        vector<coordinate> coordinateOfpass;
        
        for(int i = 0; i<map1.size(); i++){
            for(int j = 0; j < x; j++){
                if (map1[i][j]=='p'){
                    coordinateOfpass.push_back({i,j});
                }
            }
        }

        //获取start 和 end 的坐标，用struct储存一下
        vector<coordinate> startAndend; 
        startAndend.push_back(choose_start(y));
        startAndend.push_back(choose_end(y,x));

        //使用"."来表示路
        map1[startAndend[0].y][startAndend[0].x] = 'S';
        map1[startAndend[1].y][startAndend[1].x] = 'p';
        
        //转换完的坐标储存导另一个vector
        vector<coordinate> changeFinish;
        changeFinish.push_back(startAndend[0]);
        changeFinish.push_back(startAndend[1]);

        breakWall(y, x, startAndend[0].y, startAndend[0].x, map1, changeFinish);
        int a=0; 

        //储存可以用来放门的地方
        vector<coordinate> Sihutong;

        while(true){
            int times=1;
            coordinate NowCoordinate = returnTolast(changeFinish, times);
            if (NowCoordinate.y == -1 && NowCoordinate.x == -1){
                break;
            }//处理错误

            if (! breakWall(y, x, NowCoordinate.y, NowCoordinate.x, map1, changeFinish)){
                Sihutong.push_back (NowCoordinate);
                while(true){
                    times++;
                    if (NowCoordinate.y == -1 && NowCoordinate.x == -1){
                        break;
                    }//处理错误
                    coordinate NowCoordinate = returnTolast(changeFinish, times);
                    if (breakWall(y, x, NowCoordinate.y, NowCoordinate.x, map1, changeFinish)){
                        break;
                    }
                }

            }
            int havep = 0;
            for(int i = 0; i<map1.size(); i++){
                for(int j = 0; j < x; j++){
                    if (map1[i][j]=='p' || map1[i][j]=='D'){
                        havep++;
                    }
                }
            }
            if(havep==0){
                break;
            }
            a++;
        }

        coordinate NowCoordinate = returnTolast(changeFinish, 1);
        if (abs(NowCoordinate.y - startAndend[0].y)+abs(NowCoordinate.x - startAndend[0].x)<(x+y)/3){
            continue;
        }

        //制作Show_Map
        for(int i = 0; i<map1.size(); i++){
            for(int j = 0; j < x; j++){
                Show_Map[i][j] = map1[i][j];
            }
        }

        map1[NowCoordinate.y][NowCoordinate.x] = 'D';
        add_door_for_small_game(map1, Sihutong, x, y);

        add_mine (map1, changeFinish);
        return {map1, Show_Map};
    }
}
