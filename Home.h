//home.h
#ifndef HOME_H
#define HOME_H

#include <vector>
#include "player.h"

using namespace std;

void DisplayTitle();
void GameChoice();
void HomePage();
void setColor(int color);
void initConsole();
string random_tips();
void showLoadingBar();
void showLoadingScreen();
vector <int> getXY();
extern char difficulty_of_the_quanbuyouxi;


#endif
