#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct coordinate{
    int y = 0;
    int x = 0;
};

int player_heart = 0;

void getHeart(){
    cout << "请输入玩家血量（最后这个会改成让玩家选难度，然后每个难度不同血量）" << endl;
    cin >> player_heart;
    return;
}

vector <string> player_inventory (1 , "nothing");
int number_of_mineSweeping = 0;
int number_of_key = 0;
int XuYaoDe_number_of_key = 0;
coordinate Player_coordinate {0, 0};
