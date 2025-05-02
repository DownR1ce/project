# Terminal Intruder
Terminal Introder is a text-based puzzle game that combines maze, minesweeper, and various mini games.
## Team members:
Fan Yichen 3036459001
Deng Antong 3036459362
Hu Menghan 3036325652
Gao YiJia 3036452687


# Gameplay(Make sure to pull the terminal to its maximum when playing.):
The core gameplay of this game is to find all mysterious doors through the maze to obtain keys, and to ensure the survival of the character, try to avoid stepping on landmines as much as possible, and finally find the exit. This game has three built-in game difficulty levels(mazemap size: x*y): easy (11X11), normal (15X15) and hard (31X19). You also can challenge a higher level of difficulty. you can choose the size of map yourself, but the maximum size of the map is 131X21. Hidden inside the mysterious door are small games such as 2048, Snake game, Airplane Battle Game, and Sudoku. When the player enters the mysterious door, a game will randomly start, and when the player reaches a specific score, they will win the key. The difficulty of the mini game will vary with the difficulty level chosen by the player. Meanwhile, the requirements for completing different levels of difficulty vary, whether it's the main game or the mini game.

## Start Games
player need to input "make game" on the terminal, and then after few second, input "./game".

### Operation (in Home page)
After experiencing the loading page, players will enter the main page. The home page has four functions: starting a new game (N), loading stored games (S), viewing game introductions (T), and exiting games (Q). Players can choose according to their own needs.

### Operation (in main game)
'wasd' to control character (p) movement.
'q' to quit the game
'save' to save the game
'q' to exit the game
'e' to interact with the door (both for exist and enter the mysterious door)
'tips' to check the introduction
It will automatically display mines within a 5 * 5 range around the character in the lower right corner.
example for display mines:
![image](https://github.com/user-attachments/assets/84afe989-8bea-4375-9ae0-26a181a62dd7)

'm' to sweep mines (in 3 * 3)
example for mine sweep:
![image](https://github.com/user-attachments/assets/f620313a-9548-420d-83f5-a533ad9c47cc)


# Mini Games:

## Airplane Battle Game

### Introduction
"Airplane Battle" is a terminal-based shooter game developed in C++ using the ncurses library. Players control a plane moving left and right, shooting bullets to defeat enemy planes spawning at the top and moving downward. The game features three difficulty levels (easy, normal, hard), affecting enemy numbers, HP, speed, and player lives.

### Features
- Player-controlled plane with shooting capabilities.
- Enemy planes with varying HP and speeds based on difficulty.
- Three difficulty levels: Easy, Normal, Hard.
- ASCII art title and game rules display.
- Lives and HP indicators for the player.
- Win and lose conditions with options to restart, save, or exit.

#### Prerequisites
- C++ compiler (e.g., g++)
- ncurses library (Linux/macOS) or PDCurses (Windows)
- Terminal emulator

## Controls
- **Arrow Keys**: Move the plane left or right.
- **Spacebar**: Shoot bullets (up to 5 times per second).
- **Difficulty Selection**: Choose 1 (Easy), 2 (Normal), or 3 (Hard) at the start.

## License
This project is licensed under the MIT License.

## Snake Game

### Description
A classic Snake game implemented in C++11, compatible with Windows, Mac, and Ubuntu. Control the snake to eat food and grow, aiming to reach a target score to win. Features three difficulty levels with varying win conditions and food disappearance mechanics.

### Controls
- W: Move up  
- A: Move left  
- S: Move down  
- D: Move right

### Difficulty Levels
- **Easy (e):** Win at 10 points, food disappears after 25 seconds  
- **Normal (n):** Win at 20 points, food disappears after 18 seconds  
- **Hard (h):** Win at 30 points, food disappears after 10 seconds

### Game Over Options
- Retry: Start a new game with the same difficulty  
- Save: Save difficulty and score to `save.txt`  
- Quit: Exit the game

### Contribution Guidelines
- Submit issues or pull requests on the project's GitHub repository.

### License
This project is licensed under the MIT License.

### Fun Facts
Did you know the original Snake game was created in the 1970s and gained popularity on Nokia phones in the late 1990s?

## 2048

### Introduction
The 2048 mini game in Terminal Intruder is a text-based implementation of the popular tile-matching puzzle. The game is played on a 4x4 grid filled with tiles that have values of 2 or 4. The goal is to combine tiles with the same value by sliding them in four directions (up, down, left, or right) to create a tile with the value of 2048. Each move causes all tiles to slide as far as possible in the chosen direction, and tiles with matching values that are adjacent after the slide will merge into one tile with a combined value.

### Features
- **Simple Interface**: A clean text-based 4x4 grid display.
- **Intuitive Controls**: Use arrow keys (Up, Down, Left, Right) to slide the tiles.
- **Scoring System**: Points are awarded for each tile merge, and the score is displayed during the game.
- **Dynamic Difficulty**: The difficulty of the game scales with the overall difficulty level chosen for Terminal Intruder, affecting the initial tile values and the speed at which the grid fills up.

### Gameplay Rules
- **Tile Movement**: Tiles slide in the chosen direction until they hit another tile or the edge of the grid.
- **Merging Tiles**: When two tiles with the same value collide during a slide, they merge into a single tile with a value equal to the sum of the original tiles (e.g., two 2s merge into a 4, two 4s merge into an 8, and so on).
- **Game Over**: The game ends when the grid is full and no more moves are possible.
- **Win Condition**: The player wins when they successfully create a tile with the value of 2048.

### Controls
- **Arrow Keys**:
  - **Up**: Slide all tiles upwards.
  - **Down**: Slide all tiles downwards.
  - **Left**: Slide all tiles to the left.
  - **Right**: Slide all tiles to the right.

### License
This mini game is licensed under the MIT License, same as the overall Terminal Intruder project.


## Sudoku

### Introduction
The Sudoku mini game in Terminal Intruder is a traditional number-placement puzzle presented in a text-based format. The game is played on a 9x9 grid, which is further divided into nine 3x3 sub-grids. Some cells are pre-filled with numbers ranging from 1 to 9, and the player's task is to fill in the remaining empty cells. The objective is to ensure that each row, each column, and each 3x3 sub-grid contains all the numbers from 1 to 9 without any repetition.

### Features
- **Classic Puzzle**: Follows the standard Sudoku rules, providing a challenging logic-based experience.
- **Text-based Display**: The 9x9 grid is clearly shown in the terminal, with pre-filled numbers highlighted for easy identification.
- **Difficulty Scaling**: The level of difficulty adapts to the overall difficulty setting of Terminal Intruder, with harder modes featuring fewer pre-filled cells and more complex puzzles.
- **Error Detection**: The game alerts the player if they enter a number that violates the Sudoku rules (i.e., duplicates in a row, column, or sub-grid).

### Gameplay Rules
- **Filling Cells**: Players enter numbers from 1 to 9 into the empty cells of the 9x9 grid.
- **Row, Column, and Sub-grid Constraints**: Each row, column, and 3x3 sub-grid must contain all the numbers from 1 to 9 exactly once.
- **Win Condition**: The player wins when the entire 9x9 grid is filled correctly, satisfying all the Sudoku rules.
- **Game Over**: There is no traditional "game over" in Sudoku; players can continue to attempt to solve the puzzle until they complete it or choose to exit.

### Controls
- **Number Input**: Type the number (1-9) corresponding to the cell you want to fill. Use the cursor movement keys to navigate between cells.
- **Clear Input**: If you make a mistake, you can overwrite the incorrect number with the correct one.
- **Exit**: Type a specific command (e.g., "quit") to exit the Sudoku mini game and return to the main Terminal Intruder game.

### License
This Sudoku mini game is also licensed under the MIT License, consistent with the rest of the Terminal Intruder project. 



# Code
### Maps
Each of our maps is randomly generated (DFS), using two vector types (MazeMap_hide & MazeMap_show) to store hidden and displayed maps. The purpose of doing this is to hide some elements, such as mines, so that they can be easily identified character's movement.

### Player
Multiple global variables were used (extern variables in .h file), such as player's heart, number_of_minesweeping and so on. This can facilitate judgment in each different file. The player coordinates are recorded through a struct type called coordinate.

### Movement
By comparing using MazeMap_hide and MazeMap_show, determine which blocks players can move, which blocks are mines, and which blocks are doors.

### Mini Games
We have completed four different mini games and integrated them into a vector variable, which can be used to generate random numbers to achieve the method of randomly entering the mini game.

### utils
By using visual numbers to alert players of the number of mines around(5*5). The map can be centered in the terminal output.

### Ctime
Using the Ctime library to simulate loading pages

### Different Environments
The code will automatically determine the player's environment, such as Windows or iOS, and execute different code commands.

# Author's Words
"Remember, this game requires good memory and intelligence, use your brain to find the solution!!! :) I guarantee that every map has a solution. You can determine which row or column has mines by constantly moving your character and check the mine number, which is completely different from normal Minesweeping O.o, That's it, have a fun in Terminal Intruder!!"
