#include <vector>
#include <string>
#include <iostream>
#include "Savedata.h"
#include "player.h"

using namespace std;

vector <string> NameSaved;


int player_heart = 0;

void getHeart(){
    while (true){
        cout << "only positive integer!!!" << endl;
        cin >> player_heart;
        if(player_heart < 0){
            continue;
        }
        break;
    }
    return;
}

void PrintSaved(){
    showsavename(NameSaved);
    if(NameSaved.size()>0){
        for(int i = 0; i<NameSaved.size(); i++){
            cout << NameSaved[i] << endl;
        }
        cout<<"\n\n\nchoose one";
    }
    if(NameSaved.size()==0){
        cout << "There do not have your game, go to start a new game =!!";
    }
}

vector <string> player_inventory (1 , "nothing");
int number_of_mineSweeping = 0;
int number_of_key = 0;
int XuYaoDe_number_of_key = 0;
coordinate Player_coordinate {0, 0};
int number_of_mine = 0;

