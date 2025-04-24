#include <iostream>
#include <windows.h> 
#include <conio.h>   
#include <cstdlib>   
#include <ctime>    

using namespace std;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void initConsole() {
    system("mode con cols=60 lines=30");
    system("cls");
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
void showLoadingBar() {
    setColor(8); // Gray
    cout << "[";
    for (int i = 0; i < 20; i++) {
        setColor(10 + (i % 2)); // Blinking green
        cout << "■";
        Sleep(50 + rand() % 100);
    }
    setColor(8);
    cout << "]" << endl;
    setColor(7); // White
}
void showLoadingScreen() {
    initConsole();
    srand(time(0));
    
    setColor(11);
    cout << "\n\n Preparing maze adventure...\n\n";
    
    showLoadingBar();
    
    setColor(14);
    cout << "\n Loading complete!\n";
    Sleep(500);
    
    setColor(7);
    cout << "\n Press any key to continue...";
    _getch();
}

int main() {
    showLoadingScreen();
    return 0;
}

/* 
The main function here is left for testing or debugging if necessary.
int main() {
    loading();
    return 0;
}
*/
