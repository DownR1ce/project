#include "MazeMap.h"
#include <iostream>

using namespace std;

int main(){
    int x,y;
    cout<<"输入x和y构建迷宫(奇数)"<<endl;
    cin >> x >> y;
    if (x%2==0||y%2==0){
        return 0;
    }
    Maps maps = Make_Map(x, y);
    //测试部分
    for(int i = 0; i<maps.MazeMap_show.size(); i++){
        for(int j = 0; j < x; j++){
            cout << maps.MazeMap_show[i][j];
        }
        cout<<endl;
    } 
    for(int i = 0; i<maps.MazeMap_hide.size(); i++){
        for(int j = 0; j < x; j++){
            cout << maps.MazeMap_hide[i][j];
        }
        cout<<endl;
    } 
    return 0;
}
