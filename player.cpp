#include <vector>
#include <string>

using namespace std;

struct coordinate{
    int y = 0;
    int x = 0;
};

int player_heart = 0;
vector <string> player_inventory (1 , "nothing");
int number_of_mineSweeping = 0;
int number_of_key = 0;
coordinate Player_coordinate {0, 0};
