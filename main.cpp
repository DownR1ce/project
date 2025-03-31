#include "MazeMap.h"
#include <iostream>

using namespace std;

int main(){
    int x,y;
    vector<vector<char>> map1;
    cout<<"输入x和y构建迷宫(奇数 至少>7)"<<endl;
    cin >> x >> y;
    if (x%2==0||y%2==0){
        return 0;
    }
    map1 = Make_Map(x, y);
    //测试部分
    for(int i = 0; i<map1.size(); i++){
        for(int j = 0; j < x; j++){
            cout << map1[i][j];
        }
        cout<<endl;
    } 
    return 0;
}
