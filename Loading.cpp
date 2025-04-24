#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// Clear excessive progress bar
void clear_screen() {
    system("clear");
}

// Provide random tips for players to kill time while they're waiting
string random_tips() {
    vector<string> lines = {
        "You will discover that after finishing reading this line you can start to play the game.",
        "Still confused about the game? Check out our introductory video and see if you'll get inspired!",
        "Thanks for playing the game and have fun in the maze!",
        "Find the game a bit hard? Try seeking tips at home page of the game.",
        "There are actually treasures in the maze. Don't forget to find them while getting out of levels!",
        "Get stuck in the previous level? How about start a new game to refresh your mind?",
        "Fun fact: Harry Potter's Triwizard Maze in Goblet of Fire was inspired by English garden mazes.",
        "Fun fact: Hall of Mirrors mazes use angled mirrors to create infinite reflections.",
        "Find anything fun in the maze? If so, well done!",
        "Get ready for exploring the maze?"
    };
    
    srand(static_cast<unsigned int>(time(0)));
    int randomIndex = rand() % lines.size();
    return lines[randomIndex];

}

// Main body of the loading process
void loading() {
    string tip = random_tips();
    cout << "\033[5;32mLoading...\033[0m" << endl;
    
    for (int i = 0; i <= 100; i += 10) {
        clear_screen();
        
        cout << "\033[5;32mLoading...\033[0m" << endl;
        cout << endl << tip << endl;
        cout << endl;
        
        cout << "[";
        for (int j = 0; j < 10; ++j) {
            if (j < i/10) {
                cout << "=";
            } else {
                cout << " ";
            }
        }
        cout << "] " << i << "%";
        cout.flush();
        usleep(250000);
    }
    clear_screen();
    cout << "\033[32m-Loading Completed-\033[0m" << endl;
    cout << endl << "\033[5m-Press Enter to Start the Game-\033[0m" << endl;
    cin.ignore();
}

/* 
The main function here is left for testing or debugging if necessary.
int main() {
    loading();
    return 0;
}
*/
