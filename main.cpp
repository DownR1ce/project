#include "MazeMap.h"
#include "player.h"
#include "movement.h"
#include <string>
#include <iostream>

using namespace std;

int main(){
    int x,y;
    cout<<"输入x和y构建迷宫(奇数)"<<endl;
    cin >> x >> y;
    cout << "\033[2J\033[H";
    if (x%2==0||y%2==0){
        return 0;
    }
    Maps maps = Make_Map(x, y);
    //测试部分
    for(int i = 0; i<maps.MazeMap_hide.size(); i++){
        for(int j = 0; j < x; j++){
            cout << maps.MazeMap_hide[i][j];
        }
        cout<<endl;
    } 
    for(int i = 0; i<maps.MazeMap_show.size(); i++){
        for(int j = 0; j < x; j++){
            cout << maps.MazeMap_show[i][j];
        }
        cout<<endl;
    } 
    while(true){
        string order;
        cout << "please enter 'wasd' to control player" << endl;
        cin >> order;
        cout << "\033[2J\033[H";
        if (order == "e"){
            break;
        }
        if (order == "w" || order == "s" || order == "a" || order == "d"){
            player_movement(order, maps);
        }
        for(int i = 0; i<maps.MazeMap_show.size(); i++){
            for(int j = 0; j < x; j++){
                cout << maps.MazeMap_show[i][j];
            }
            cout<<endl;
        }
    }
    return 0;
}
