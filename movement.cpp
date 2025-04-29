#include <string>
#include <iostream>
#include <cmath>
#include "player.h"
#include "MazeMap.h"
#include "utils.h"
#include "SmallGame.h"

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN() system("cls")
#else
    #include <unistd.h>
    #define CLEAR_SCREEN() system("clear")
#endif

using namespace std;

int have_mine (Maps maps){
    int number = 0;
    if (maps.MazeMap_hide[Player_coordinate.y][(Player_coordinate.x-1)] == 'X'){
        number ++;
    }
    if (maps.MazeMap_hide[Player_coordinate.y][(Player_coordinate.x+1)] == 'X'){
        number ++;
    }
    if (maps.MazeMap_hide[Player_coordinate.y-1][(Player_coordinate.x-1)] == 'X'){
        number ++;
    }
    if (maps.MazeMap_hide[Player_coordinate.y-1][(Player_coordinate.x+1)] == 'X'){
        number ++;
    }
    if (maps.MazeMap_hide[Player_coordinate.y-1][(Player_coordinate.x)] == 'X'){
        number ++;
    }
    if (maps.MazeMap_hide[Player_coordinate.y+1][(Player_coordinate.x-1)] == 'X'){
        number ++;
    }
    if (maps.MazeMap_hide[Player_coordinate.y+1][(Player_coordinate.x+1)] == 'X'){
        number ++;
    }
    if (maps.MazeMap_hide[Player_coordinate.y+1][(Player_coordinate.x)] == 'X'){
        number ++;
    }
    if (maps.MazeMap_hide[Player_coordinate.y][(Player_coordinate.x+1)] != '@'){
        if (maps.MazeMap_hide[Player_coordinate.y][(Player_coordinate.x+2)] == 'X'){
            number ++;
        }
        if (maps.MazeMap_hide[Player_coordinate.y-1][(Player_coordinate.x+2)] == 'X'){
            number ++;
        }
        if (maps.MazeMap_hide[Player_coordinate.y+1][(Player_coordinate.x+2)] == 'X'){
            number ++;
        }
    }
    if (maps.MazeMap_hide[Player_coordinate.y][(Player_coordinate.x-1)] != '@'){
        if (maps.MazeMap_hide[Player_coordinate.y][(Player_coordinate.x-2)] == 'X'){
            number ++;
        }
        if (maps.MazeMap_hide[Player_coordinate.y-1][(Player_coordinate.x-2)] == 'X'){
            number ++;
        }
        if (maps.MazeMap_hide[Player_coordinate.y+1][(Player_coordinate.x-2)] == 'X'){
            number ++;
        }
    }
    if (maps.MazeMap_hide[Player_coordinate.y-1][(Player_coordinate.x)] != '@'){
        if (maps.MazeMap_hide[Player_coordinate.y-2][(Player_coordinate.x+1)] == 'X'){
            number ++;
        }
        if (maps.MazeMap_hide[Player_coordinate.y-2][(Player_coordinate.x)] == 'X'){
            number ++;
        }
        if (maps.MazeMap_hide[Player_coordinate.y-2][(Player_coordinate.x-1)] == 'X'){
            number ++;
        }
        if (maps.MazeMap_hide[Player_coordinate.y][(Player_coordinate.x+1)] != '@'){
            if (maps.MazeMap_hide[Player_coordinate.y-2][(Player_coordinate.x+2)] == 'X'){
                number ++;
            }
        }
        if (maps.MazeMap_hide[Player_coordinate.y][(Player_coordinate.x-1)] != '@'){
            if (maps.MazeMap_hide[Player_coordinate.y-2][(Player_coordinate.x-2)] == 'X'){
                number ++;
            }
        }
    }
    if (maps.MazeMap_hide[Player_coordinate.y+1][(Player_coordinate.x)] != '@'){
        if (maps.MazeMap_hide[Player_coordinate.y+2][(Player_coordinate.x+1)] == 'X'){
            number ++;
        }
        if (maps.MazeMap_hide[Player_coordinate.y+2][(Player_coordinate.x)] == 'X'){
            number ++;
        }
        if (maps.MazeMap_hide[Player_coordinate.y+2][(Player_coordinate.x-1)] == 'X'){
            number ++;
        }
        if (maps.MazeMap_hide[Player_coordinate.y][(Player_coordinate.x+1)] != '@'){
            if (maps.MazeMap_hide[Player_coordinate.y+2][(Player_coordinate.x+2)] == 'X'){
                number ++;
            }
        }
        if (maps.MazeMap_hide[Player_coordinate.y][(Player_coordinate.x-1)] != '@'){
            if (maps.MazeMap_hide[Player_coordinate.y+2][(Player_coordinate.x-2)] == 'X'){
                number ++;
            }
        }
    }
    
    return number;
}

string player_movement (string order, Maps& maps){
    coordinate maybe;
    if (order == "w"){
        maybe = {Player_coordinate.y - 1 , Player_coordinate.x};
    }
    if (order == "s") {
        maybe = {Player_coordinate.y + 1 , Player_coordinate.x};
    }
    if (order == "a"){
        maybe = {Player_coordinate.y, Player_coordinate.x - 1};
    }
    if (order == "d"){
        maybe = {Player_coordinate.y, Player_coordinate.x + 1};
    }
    string tips;
    if (maps.MazeMap_show[maybe.y][maybe.x] != '#' || maps.MazeMap_show[maybe.y][maybe.x] != '@'){
        if (maps.MazeMap_hide[maybe.y][maybe.x] == '.' || maps.MazeMap_hide[maybe.y][maybe.x] == 'S' || maps.MazeMap_hide[maybe.y][maybe.x] == 'O'){
            if (maps.MazeMap_hide[Player_coordinate.y][Player_coordinate.x] == '.' || maps.MazeMap_hide[Player_coordinate.y][Player_coordinate.x] == 'S'){
                maps.MazeMap_show [Player_coordinate.y][Player_coordinate.x] = '.';
                Player_coordinate = maybe;
                maps.MazeMap_show[maybe.y][maybe.x] = 'P';
            }
            if (maps.MazeMap_hide[Player_coordinate.y][Player_coordinate.x] == 'O'){
                maps.MazeMap_show [Player_coordinate.y][Player_coordinate.x] = 'O';
                Player_coordinate = maybe;
                maps.MazeMap_show[maybe.y][maybe.x] = 'P';
            }
            maps.MazeMap_show [Player_coordinate.y][Player_coordinate.x] = '.';
            Player_coordinate = maybe;
            maps.MazeMap_show[maybe.y][maybe.x] = 'P';
        }
        if (maps.MazeMap_hide[maybe.y][maybe.x] == 'X'){
            tips = "You stepped on a bomb, -1 heart";
            number_of_mine -= 1;
            maps.MazeMap_show [Player_coordinate.y][Player_coordinate.x] = '.';
            Player_coordinate = maybe;
            maps.MazeMap_show[maybe.y][maybe.x] = 'X';
            maps.MazeMap_hide[maybe.y][maybe.x] = '.'; // 踩到炸弹后，炸弹失效，炸掉一滴血。
            player_heart --;
        }
        if (maps.MazeMap_hide[maybe.y][maybe.x] == '?'){
            tips = "You have found the mysterious door, press' e 'to enter, \nand enter' other 'to cancel the interaction.";
            cout << tips<<endl;
            maps.MazeMap_show[maybe.y][maybe.x] = '?';
            char o;
            int temK = number_of_key;
            cin >> o;
            if (o=='e'){
                int type = startSmallGame();
            }
            if (o!='e'){
                CLEAR_SCREEN();
            }
            if (number_of_key > temK){
                maps.MazeMap_show[maybe.y][maybe.x] = '¿';
                maps.MazeMap_hide[maybe.y][maybe.x] = '¿';
            }
        }
        if (maps.MazeMap_hide[maybe.y][maybe.x] == 'D'){
            tips = "You found the door, but where's the key? \nPress' e 'to unlock, press' other 'to cancel the interaction.";
            maps.MazeMap_show[maybe.y][maybe.x] = 'D';
        }
    }
    else{
        tips = "you can not move to the wall";
    }
    return tips;
}

bool TongGuan(char O){
    if(O == 'e'){
        if (number_of_key == XuYaoDe_number_of_key){
            cout << "congratulations!!! You successfully escaped" << endl;
            return true;
        }
        else{
            cout << "sorry, you do not have enough key" << endl;
            return false;
        }
    }
    else{
        return false;
    }
}

void Saolei(string order, Maps& map){
    int numberofmine = 0;
    if(number_of_mineSweeping == 0){
        cout << "sorry, you cannot sweep mine anmymore!!"<<endl;
        return;
    }
    number_of_mineSweeping -= 1;
    if(order == "m"){
        if (map.MazeMap_hide[Player_coordinate.y][Player_coordinate.x + 1] == 'X'){
            map.MazeMap_hide[Player_coordinate.y][Player_coordinate.x + 1]='O';
            map.MazeMap_show[Player_coordinate.y][Player_coordinate.x + 1]='O';
            numberofmine++;
        }
        if (map.MazeMap_hide[Player_coordinate.y][Player_coordinate.x - 1] == 'X'){
            map.MazeMap_hide[Player_coordinate.y][Player_coordinate.x - 1]='O';
            map.MazeMap_show[Player_coordinate.y][Player_coordinate.x - 1]='O';
            numberofmine++;
        }
        if (map.MazeMap_hide[Player_coordinate.y + 1][Player_coordinate.x + 1] == 'X'){
            map.MazeMap_hide[Player_coordinate.y + 1][Player_coordinate.x + 1]='O';
            map.MazeMap_show[Player_coordinate.y + 1][Player_coordinate.x + 1]='O';
            numberofmine++;
        }
        if (map.MazeMap_hide[Player_coordinate.y + 1][Player_coordinate.x - 1] == 'X'){
            map.MazeMap_hide[Player_coordinate.y + 1][Player_coordinate.x - 1]='O';
            map.MazeMap_show[Player_coordinate.y + 1][Player_coordinate.x - 1]='O';
            numberofmine++;
        }
        if (map.MazeMap_hide[Player_coordinate.y + 1][Player_coordinate.x] == 'X'){
            map.MazeMap_hide[Player_coordinate.y + 1][Player_coordinate.x]='O';
            map.MazeMap_show[Player_coordinate.y + 1][Player_coordinate.x]='O';
            numberofmine++;
        } 
        if (map.MazeMap_hide[Player_coordinate.y - 1][Player_coordinate.x + 1] == 'X'){
            map.MazeMap_hide[Player_coordinate.y - 1][Player_coordinate.x + 1]='O';
            map.MazeMap_show[Player_coordinate.y - 1][Player_coordinate.x + 1]='O';
            numberofmine++;
        }
        if (map.MazeMap_hide[Player_coordinate.y - 1][Player_coordinate.x - 1] == 'X'){
            map.MazeMap_hide[Player_coordinate.y - 1][Player_coordinate.x - 1]='O';
            map.MazeMap_show[Player_coordinate.y - 1][Player_coordinate.x - 1]='O';
            numberofmine++;
        }
        if (map.MazeMap_hide[Player_coordinate.y - 1][Player_coordinate.x] == 'X'){
            map.MazeMap_hide[Player_coordinate.y - 1][Player_coordinate.x]='O';
            map.MazeMap_show[Player_coordinate.y - 1][Player_coordinate.x]='O';
            numberofmine++;
        } 
        number_of_mine -= numberofmine;
    }
}

