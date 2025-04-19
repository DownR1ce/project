#include "MazeMap.h"
#include "player.h"
#include "movement.h"
#include <string>
#include <vector>
#include <iostream>
#include "utils.h"
#include "SmallGame.h"

using namespace std;

int main(){
    int x,y;
    cout<<"输入x和y构建迷宫(奇数)"<<endl;
    cin >> x >> y;
    getHeart();
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

    vector <string> Map_Output_vector;

    for(int i = 0; i<maps.MazeMap_show.size(); i++){
        string output = "";
        for(int j = 0; j < x; j++){
            output += maps.MazeMap_show[i][j];
        }
        Map_Output_vector.push_back (output);
    } 
    printCentered(Map_Output_vector);
    int Mine_Number = have_mine (maps);
    Print_Number (Mine_Number);

    while(true){
        string order;
        string tips;
        cout << "please enter 'wasd' to control player" << endl;
        cin >> order;
        cout << "\033[2J\033[H";
        if (order == "q"){
            break;
        }
        if (order == "w" || order == "s" || order == "a" || order == "d"){
            tips = player_movement(order, maps);
        }
        vector <string> Map_Output_vector;
        for(int i = 0; i<maps.MazeMap_show.size(); i++){
            string output = "";
            for(int j = 0; j < x; j++){
                output += maps.MazeMap_show[i][j];
            }
            Map_Output_vector.push_back (output);
        } 

        printCentered(Map_Output_vector);
        cout << tips << endl;

        int Mine_Number = have_mine (maps);
        Print_Number (Mine_Number);
        
        //神秘の门
        if (tips == "你找到了神秘的门, 按e进入"){
            char o;
            cin >> o;
            if (o=='e'){
                JiaZhuangTaShiYiGeYouXi ();
            }
        }

        //出去の门
        if (tips == "你找到了出口，但是钥匙呢？\n 摁e开锁"){
            char O;
            cin >> O;
            if (TongGuan(O)){
                break;
            }
        }

        if (player_heart == 0){
            cout << "game over, 你死了" << endl;
            break;
        }
    }
    return 0;
}
