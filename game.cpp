#include "MazeMap.h"
#include "player.h"
#include "movement.h"
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "utils.h"
#include "SmallGame.h"
#include "home.h"
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep(ms * 1000)
#endif

using namespace std;

bool game(){
    vector <int> xy = getXY();
    int x,y;
    x= xy[0];
    y= xy[1];
    if (x==100000 && y==100000){
        return true;
    }
    Maps maps = Make_Map(x, y);
    cout << "the map size is "<<x<<" * "<<y<<", please ready to play!!!"<<endl; 
    SLEEP(5000);
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
        cout << "number of mine remain: "<< number_of_mine << endl;
        cout << "number of mine sweeping remain: " <<  number_of_mineSweeping << endl;
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
        if (order == "m"){
            Saolei(order, maps);
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
    return true;
}
