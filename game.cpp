#include "MazeMap.h"
#include "player.h"
#include "movement.h"
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "utils.h"
#include "SmallGame.h"
#include "Home.h"
#include "Savedata.h"
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
    #define CLEAR_SCREEN() system("cls")
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep(ms * 1000)
    #define CLEAR_SCREEN() system("clear")
#endif

using namespace std;

bool game(string ch){
    Maps maps;
    int x, y;
    if (ch == "n"){
        number_of_key = 0;
        vector <int> xy = getXY();
        x= xy[0];
        y= xy[1];
        if (x==100000 && y==100000){
            return true;
            }
        maps = Make_Map(x, y);
        cout << "the map size is "<<x<<" * "<<y<<", please ready to play!!!"<<endl; 
        SLEEP(5000);
    }
    else{
        GameSaveData data;
        if (!loadGame(data, ch)) {
            cerr << "Failed to load archive！" << endl;
            return true;
        }
        player_heart = data.player_heart;
        player_inventory = data.player_inventory;
        number_of_mineSweeping = data.number_of_mineSweeping;
        number_of_key = data.number_of_key;
        XuYaoDe_number_of_key = data.XuYaoDe_number_of_key;
        Player_coordinate.x = data.Player_coordinate.x;
        Player_coordinate.y = data.Player_coordinate.y;
        number_of_mine = data.number_of_mine;
        difficulty_of_the_quanbuyouxi = data.difficulty_of_the_quanbuyouxi;
        maps.MazeMap_hide = data.MazeMap_hide;
        maps.MazeMap_show = data.MazeMap_show;
        x = data.x;
        y = data.y;
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
        cout << "your heart: " << player_heart << endl;
        cout << "your key number is: " << number_of_key << endl;
        cout << "number of mine remain: "<< number_of_mine << endl;
        cout << "number of mine sweeping remain: " <<  number_of_mineSweeping << endl;
        string order;
        string tips;
        cout << "please enter 'wasd' to control player" << endl;
        cout <<"enter'save' to save the game" << endl;
        cin >> order;
        cout << "\033[2J\033[H";
        if (order == "save"){
            GameSaveData savedata;
            savedata.player_heart = player_heart;
            savedata.player_inventory=player_inventory;
            savedata.number_of_mineSweeping = number_of_mineSweeping;
            savedata.number_of_key = number_of_key;
            savedata.XuYaoDe_number_of_key = XuYaoDe_number_of_key;
            savedata.Player_coordinate.x = Player_coordinate.x;
            savedata.Player_coordinate.y = Player_coordinate.y;
            savedata.number_of_mine = number_of_mine;
            savedata.difficulty_of_the_quanbuyouxi = difficulty_of_the_quanbuyouxi;
            savedata.x = x;
            savedata.y = y;
            savedata.MazeMap_hide = maps.MazeMap_hide;
            savedata.MazeMap_show = maps.MazeMap_show;
            string name;
            cout << "please enter a name for your savefile." << endl;
            cin >> name;
            saveGame(savedata, name);
            savenameintofile(name);
            NameSaved.push_back(name);
            break;
        }
        if(order == "FYCShiGeDaShuaiBi"){
            player_heart=10000;
        }
        if (order == "q"){
            break;
        }
        if (order == "w" || order == "s" || order == "a" || order == "d"){
            tips = player_movement(order, maps);
        }
        if (order == "m"){
            Saolei(order, maps);
        }
        if (order == "tips"){
            CLEAR_SCREEN();
            tips123();
            string asbd;
            if (cin >> asbd){
                CLEAR_SCREEN();
            }
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
        if (tips == "You have found the mysterious door, press' e 'to enter, \nand enter' other 'to cancel the interaction."){
            int abcdefg = 1;
        }

        //出去の门
        if (tips == "You found the door, but where's the key? \nPress' e 'to unlock, press' other 'to cancel the interaction."){
            char O;
            cin >> O;
            if (TongGuan(O)){
                break;
            }
        }
        if (player_heart == 0){
            cout << "game over, you X_X" << endl;
            break;
        }
    }
    return true;
}
