//movement.h
#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <string>
#include "player.h"
#include "MazeMap.h"

string player_movement (string order, Maps& maps);
int have_mine (Maps maps);
bool TongGuan(char O);

#endif
