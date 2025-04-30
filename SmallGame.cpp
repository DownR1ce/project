#include <iostream>
#include "player.h"
#include <vector>
#include <ncurses.h>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <deque>
#include <random>
#include <limits>
#include <ios>
#include <fstream>
#include "Element.h"
#include "Home.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#endif

using namespace std;

//这个文件到时候用来装小游戏用，通关获得钥匙的那个小游戏

// Game structs
struct Plane {
    int x, y;
    int hp;
};

struct Bullet {
    int x, y;
};

// Game state
int max_y, max_x;
Plane player;
std::vector<Plane> enemies;
std::vector<Bullet> bullets;
int player_lives;
int defeated_count;
int total_to_defeat;
int enemy_hp;
float enemy_speed; // rows per second
int bullets_per_shot;
float spawn_timer;
float shoot_timer;
bool game_running;

// Difficulty settings
struct Difficulty {
    int lives;
    int enemies_to_defeat;
    int enemy_hp;
    float enemy_speed;
    int bullets_per_shot;
};

void setup_difficulty(char choice) {
    Difficulty diff;
    switch (choice) {
        case '1': // Easy
            diff = {5, 10, 1, 1.0, 7};
            break;
        case '2': // Normal
            diff = {3, 20, 2, 2.0, 7};
            break;
        case '3': // Hard
            diff = {1, 30, 3, 3.0, 7};
            break;
        default:
            diff = {5, 10, 1, 1.0, 7};
    }
    player_lives = diff.lives;
    total_to_defeat = diff.enemies_to_defeat;
    enemy_hp = diff.enemy_hp;
    enemy_speed = diff.enemy_speed;
    bullets_per_shot = diff.bullets_per_shot;
}

void display_rules() {
    clear();
    mvprintw(0, 0, "Game Rules:");
    mvprintw(1, 0, "1. Enemy planes spawn at the top and move down.");
    mvprintw(2, 0, "2. Move your plane left/right with arrow keys.");
    mvprintw(3, 0, "3. Press space to shoot bullets (up to 5 per second).");
    mvprintw(4, 0, "4. Bullets hitting enemies reduce their HP.");
    mvprintw(5, 0, "5. Enemy collision with your plane reduces your HP.");
    mvprintw(6, 0, "6. Win by defeating all enemies; lose if all planes are destroyed.");
    mvprintw(8, 0, "Choose difficulty: 1. Easy 2. Normal 3. Hard");
    refresh();
    nodelay(stdscr, FALSE); // Make getch() blocking
    nodelay(stdscr, TRUE); // Restore non-blocking
    setup_difficulty(difficulty_of_the_quanbuyouxi);
}

void display_game_over(bool won) {
    clear();
    nodelay(stdscr, FALSE); // Make getch() blocking
    if (won) {
        mvprintw(max_y / 2, max_x / 2 - 5, "Victory! All enemies defeated!");
        mvprintw(max_y / 2 + 2, max_x / 2 - 10, "Press any key to exit.");
        number_of_key++;
        refresh();
        getch();
        endwin();
        return;
    } else {
        mvprintw(max_y / 2, max_x / 2 - 5, "BOOM! Game Over!");
        mvprintw(max_y / 2 + 2, max_x / 2 - 10, "1. Play again 2. Save 3. Exit");
        refresh();
        int choice;
        do {
            choice = getch() - '0';
            if (choice == 3) {
                mvprintw(max_y / 2 + 4, max_x / 2 - 15, "Exit will lose all data and you will not get the key. Confirm? (y/n)");
                refresh();
                int confirm = getch();
                if (confirm == 'y') {
                    endwin();
                    return;
                }
            }
        } while (choice != 1 && choice != 2);
        if (choice == 2) {
            // Placeholder for save functionality
            mvprintw(max_y / 2 + 4, max_x / 2 - 10, "Game saved (placeholder).");
            refresh();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        // Restart
        player = {(max_x - 5) / 2, max_y - 3, 5};
        enemies.clear();
        bullets.clear();
        defeated_count = 0;
        spawn_timer = 0;
        shoot_timer = 0;
        game_running = true;
        nodelay(stdscr, TRUE); // Restore non-blocking for game loop
        display_rules();
    }
}

void AirplaneBattleGame() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    getmaxyx(stdscr, max_y, max_x);
    srand(static_cast<unsigned int>(time(0)));

    // Show title and rules
    display_rules();

    // Initialize game state
    player = {(max_x - 5) / 2, max_y - 3, 5};
    defeated_count = 0;
    spawn_timer = 0;
    shoot_timer = 0;
    game_running = true;

    // Game loop
    auto last_time = std::chrono::steady_clock::now();
    while (game_running) {
        auto current_time = std::chrono::steady_clock::now();
        float delta_time = std::chrono::duration<float>(current_time - last_time).count();
        last_time = current_time;

        // Handle input
        int ch = getch();
        if (ch != ERR) {
            switch (ch) {
                case KEY_LEFT:
                    if (player.x > 0) player.x--;
                    break;
                case KEY_RIGHT:
                    if (player.x < max_x - 5) player.x++;
                    break;
                case ' ':
                    if (shoot_timer <= 0) {
                        for (int i = -bullets_per_shot / 2; i <= bullets_per_shot / 2; i++) {
                            Bullet b = {player.x + i, player.y - 1};
                            if (b.x >= 0 && b.x < max_x) bullets.push_back(b);
                        }
                        shoot_timer = 0.2; // 5 shots per second
                    }
                    break;
            }
        }

        // Update timers
        spawn_timer -= delta_time;
        shoot_timer -= delta_time;

        // Spawn enemies
        if (spawn_timer <= 0 && enemies.size() < 10 && defeated_count < total_to_defeat) {
            int x = rand() % max_x;
            Plane enemy = {x, 0, enemy_hp};
            enemies.push_back(enemy);
            spawn_timer = 1.0;
        }

        // Move bullets
        for (auto& b : bullets) {
            b.y--;
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.y < 0; }), bullets.end());

        // Move enemies
        static float enemy_move_timer = 0;
        enemy_move_timer += delta_time;
        if (enemy_move_timer >= 1.0 / enemy_speed) {
            for (auto& e : enemies) {
                e.y++;
            }
            enemy_move_timer = 0;
        }
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const Plane& e) { return e.y >= max_y; }), enemies.end());

        // Check collisions
        for (auto& b : bullets) {
            for (auto& e : enemies) {
                if (b.x == e.x && b.y == e.y) {
                    e.hp--;
                    if (e.hp <= 0) {
                        defeated_count++;
                        e.y = -1;
                    }
                    b.y = -1;
                }
            }
        }
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const Plane& e) { return e.y < 0; }), enemies.end());
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.y < 0; }), bullets.end());

        // Check player-enemy collisions
        for (auto& e : enemies) {
            if (e.x >= player.x && e.x <= player.x + 4 && e.y >= player.y && e.y <= player.y + 2) {
                player.hp--;
                e.y = -1;
                if (player.hp <= 0) {
                    player_lives--;
                    if (player_lives > 0) {
                        player = {(max_x - 5) / 2, max_y - 3, 5};
                    } else {
                        game_running = false;
                        display_game_over(false);
                    }
                }
            }
        }

        // Draw screen
        clear();
        // Draw player as a 5x3 block
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 5; dx++) {
                mvaddch(player.y + dy, player.x + dx, '@');
            }
        }
        for (const auto& e : enemies) {
            mvaddch(e.y, e.x, 'V');
        }
        for (const auto& b : bullets) {
            mvaddch(b.y, b.x, '*');
        }
        mvprintw(0, max_x - 20, "Lives: %d HP: %d", player_lives, player.hp);
        mvprintw(1, max_x - 20, "Defeated: %d/%d", defeated_count, total_to_defeat);
        refresh();

        // Check win condition
        if (defeated_count >= total_to_defeat) {
            game_running = false;
            display_game_over(true);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    endwin();
    return;
}

/*
 * Cross-Platform Snake Game
 *
 * This program implements a classic Snake game using C++11, compatible with
 * Windows, Mac, and Ubuntu without external libraries. The player controls a snake
 * to eat food, aiming to reach a target score to win. The game supports three
 * difficulty levels, with varying win scores and food disappearance times.
 *
 * Compilation:
 *   - Windows: g++ main.cpp -o snake
 *   - Mac/Ubuntu: g++ -std=c++11 snake.cpp -o snake
 *
 * Running:
 *   - Windows: snake.exe
 *   - Mac/Ubuntu: ./snake
 *
 * Controls:
 *   - W: Move up
 *   - A: Move left
 *   - S: Move down
 *   - D: Move right
 *
 * Difficulty Levels:
 *   - Easy (e): Win at 10 points, food disappears after 10 seconds (100 ticks)
 *   - Normal (n): Win at 20 points, food disappears after 5 seconds (50 ticks)
 *   - Hard (h): Win at 30 points, food disappears after 2.5 seconds (25 ticks)
 *
 * On game over (e.g., hitting a wall or self), the player can:
 *   - Retry: Start a new game with the same difficulty
 *   - Save: Save difficulty and score to save.txt
 *   - Quit: Exit the game
 */
 
 // Game board dimensions
 const int WIDTH = 20;
 const int HEIGHT = 20;
 
 // Possible directions for the snake
 enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
 
 // Represents a position on the game board (row, column)
 struct Position {
     int row;
     int col;
 };
 
 // Draws the initial game board with borders
 void drawInitialBoard() {
     cout << "\033[2J\033[H";  // Clear screen and move to top
     cout << "Score: 0 Direction: UP" << endl;
     for (int i = 0; i < WIDTH + 2; i++) cout << "#";
     cout << endl;
     for (int row = 0; row < HEIGHT; row++) {
         cout << "#";
         for (int col = 0; col < WIDTH; col++) {
             cout << ' ';
         }
         cout << "#" << endl;
     }
     for (int i = 0; i < WIDTH + 2; i++) cout << "#";
     cout << endl;
     cout.flush();
 }
 
 // Updates the game board, redrawing only changed parts for smoother animation
 void updateBoard(const vector<char>& board, const deque<Position>& snake, const Position& food_pos,
                 int score, Direction dir, vector<char>& prev_board, deque<Position>& prev_snake,
                 Position& prev_food_pos, int& prev_score, Direction& prev_dir) {
     // Update score and direction if changed
     if (score != prev_score || dir != prev_dir) {
         cout << "\033[1;1H";  // Move to row 1, column 1
         cout << "Score: " << score << " Direction: ";
         switch (dir) {
             case LEFT: cout << "LEFT"; break;
             case RIGHT: cout << "RIGHT"; break;
             case UP: cout << "UP"; break;
             case DOWN: cout << "DOWN"; break;
             default: cout << "STOP"; break;
         }
         cout << string(20, ' ');  // Clear rest of line
     }
 
     // Update changed cells
     // New head
     Position head = snake.front();
     cout << "\033[" << head.row + 3 << ";" << head.col + 2 << "H@";
 
     // Old tail (if no food eaten)
     if (snake.size() == prev_snake.size() && !snake.empty()) {
         Position tail = prev_snake.back();
         cout << "\033[" << tail.row + 3 << ";" << tail.col + 2 << "H ";
     }
 
     // New food (if position changed)
     if (food_pos.row != prev_food_pos.row || food_pos.col != prev_food_pos.col) {
         cout << "\033[" << food_pos.row + 3 << ";" << food_pos.col + 2 << "H*";
     }
 
     // Update tail symbol
     if (snake.size() > 1) {
         Position tail = snake.back();
         cout << "\033[" << tail.row + 3 << ";" << tail.col + 2 << "H=";
     }
 
     cout.flush();  // Ensure immediate display
 
     // Update previous frame state
     prev_board = board;
     prev_snake = snake;
     prev_food_pos = food_pos;
     prev_score = score;
     prev_dir = dir;
 }
 
 // Places a new food item at a random empty position
 void placeFood(vector<char>& board, const deque<Position>& snake, Position& food_pos,
                default_random_engine& rng, uniform_int_distribution<int>& dist_row,
                uniform_int_distribution<int>& dist_col) {
     while (true) {
         int row = dist_row(rng);
         int col = dist_col(rng);
         if (board[row * WIDTH + col] == ' ') {
             food_pos = {row, col};
             board[row * WIDTH + col] = '*';
             break;
         }
     }
 }
 
 #ifdef _WIN32
 // Windows: Check if a key is pressed
 bool keyPressed() {
     return _kbhit() != 0;
 }
 
 // Windows: Get the pressed key
 char getKey() {
     return _getch();
 }
 #else
 // Unix-like: Store original terminal settings
 struct termios original_term;
 
 // Unix-like: Set non-canonical mode for non-blocking input
 void setNonCanonicalMode() {
     tcgetattr(STDIN_FILENO, &original_term);
     struct termios term = original_term;
     term.c_lflag &= ~(ICANON | ECHO);
     tcsetattr(STDIN_FILENO, TCSANOW, &term);
 }
 
 // Unix-like: Restore original terminal settings
 void resetTerminal() {
     tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
 }
 
 // Unix-like: Check if a key is pressed
 bool keyPressed() {
     fd_set set;
     FD_ZERO(&set);
     FD_SET(STDIN_FILENO, &set);
     struct timeval timeout = {0, 0};
     int rv = select(STDIN_FILENO + 1, &set, nullptr, nullptr, &timeout);
     return rv > 0;
 }
 
 // Unix-like: Get the pressed key
 char getKey() {
     char c;
     if (read(STDIN_FILENO, &c, 1) == 1) {
         return c;
     }
     return 0;
 }
 #endif
 
 // Main function: Game entry point
 void snake() {
     // Display game instructions and wait for confirmation
     cout << "Welcome to the Snake Game!\n";
     cout << "Rules:\n";
     cout << "- Use W/A/S/D keys to move the snake up/left/down/right.\n";
     cout << "- Eating food (*) will increase length and score.\n";
     cout << "- Avoid hitting walls or your own body.\n";
     cout << "- Reach the target score based on difficulty to win.\n";
     cout << "Press Enter to continue...\n";
 
     // Prompt for difficulty selection
     char difficulty;
     difficulty = difficulty_of_the_quanbuyouxi;
     cin.ignore(numeric_limits<streamsize>::max(), '\n');
 
     // Set target score, movement delay, and food disappearance time
     int required_score;
     int move_delay;
     int food_disappear_time;
     switch (difficulty) {
         case '1':
             required_score = 3;
             move_delay = 1000;  // 1 second
             food_disappear_time = 25000;  // 25 seconds
             break;
         case '2':
             required_score = 5;
             move_delay = 700;  // 0.7 seconds
             food_disappear_time = 18000;  // 18 seconds
             break;
         case '3':
             required_score = 8;
             move_delay = 500;  // 0.5 seconds
             food_disappear_time = 10000;  // 10 seconds
             break;
         default:
             cout << "Invalid choice, defaulting to normal." << endl;
             required_score = 20;
             move_delay = 700;
             food_disappear_time = 18000;
             difficulty = 'n';
             break;
     }
 
     // Initialize random number generator with cast to avoid warning
     default_random_engine rng(static_cast<default_random_engine::result_type>(time(nullptr)));
     uniform_int_distribution<int> dist_row(0, HEIGHT - 1);
     uniform_int_distribution<int> dist_col(0, WIDTH - 1);
 
     // Outer loop for retry functionality
     bool continue_playing = true;
     while (continue_playing) {
         #ifndef _WIN32
         setNonCanonicalMode();
         #endif
 
         // Initialize game state
         vector<char> board(HEIGHT * WIDTH, ' ');
         deque<Position> snake = { {10, 9}, {10, 10} };  // Head at {10,9}, tail at {10,10}
         board[snake[0].row * WIDTH + snake[0].col] = '@';
         board[snake[1].row * WIDTH + snake[1].col] = '=';
         Direction dir = UP;  // Initial direction upward
         Position food_pos;
         placeFood(board, snake, food_pos, rng, dist_row, dist_col);
         auto food_placed_time = chrono::steady_clock::now();  // Record food placement time
         int score = 0;
         bool game_over = false;
         bool win = false;
 
         // Previous frame state for incremental updates
         vector<char> prev_board = board;
         deque<Position> prev_snake = snake;
         Position prev_food_pos = food_pos;
         int prev_score = -1;
         Direction prev_dir = STOP;
 
         // Draw initial board
         drawInitialBoard();
         cout << "\033[" << food_pos.row + 3 << ";" << food_pos.col + 2 << "H*";
         cout << "\033[" << snake[0].row + 3 << ";" << snake[0].col + 2 << "H@";
         cout << "\033[" << snake[1].row + 3 << ";" << snake[1].col + 2 << "H=";
         cout.flush();
         
         auto last_move_time = chrono::steady_clock::now();
         const int sleep_delay = 20; // 毫秒
         
         // Main game loop
         while (!game_over) {
             auto current_time = chrono::steady_clock::now();
             updateBoard(board, snake, food_pos, score, dir, prev_board, prev_snake, prev_food_pos, prev_score, prev_dir);
             
             // Handle keyboard input
             if (keyPressed()) {
                 char key = getKey();
                 if (key == 'a' && dir != RIGHT) dir = LEFT;
                 else if (key == 'd' && dir != LEFT) dir = RIGHT;
                 else if (key == 'w' && dir != DOWN) dir = UP;
                 else if (key == 's' && dir != UP) dir = DOWN;
                 // Calculate new head position
                 Position head = snake.front();
                 Position new_head = head;
                 if (dir == LEFT) new_head.col--;
                 else if (dir == RIGHT) new_head.col++;
                 else if (dir == UP) new_head.row--;
                 else if (dir == DOWN) new_head.row++;
                 // Check for collisions
                 if (new_head.row < 0 || new_head.row >= HEIGHT ||
                     new_head.col < 0 || new_head.col >= WIDTH ||
                     (board[new_head.row * WIDTH + new_head.col] == '@' || board[new_head.row * WIDTH + new_head.col] == '=')) {
                     game_over = true;
                     win = false;
                 } else {
                     // Check if food is eaten
                     bool ate_food = false;
                     if (new_head.row == food_pos.row && new_head.col == food_pos.col) {
                         ate_food = true;
                         score++;
                         placeFood(board, snake, food_pos, rng, dist_row, dist_col);
                         food_placed_time = chrono::steady_clock::now();  // Reset food timer
                     }
                     
                     // Move snake
                     snake.push_front(new_head);
                     board[new_head.row * WIDTH + new_head.col] = '@';
                     if (!ate_food) {
                         Position tail = snake.back();
                         snake.pop_back();
                         board[tail.row * WIDTH + tail.col] = ' ';
                     }
                     
                     // Check if food should disappear
                     auto current_time = chrono::steady_clock::now();
                     if (chrono::duration_cast<chrono::milliseconds>(current_time - food_placed_time).count() >= food_disappear_time) {
                         board[food_pos.row * WIDTH + food_pos.col] = ' ';
                         placeFood(board, snake, food_pos, rng, dist_row, dist_col);
                         food_placed_time = chrono::steady_clock::now();
                     }
                     
                     // Check win condition
                     if (score >= required_score) {
                         game_over = true;
                         win = true;
                     }
                     
                 }
             }
             else if(chrono::duration_cast<chrono::milliseconds>(current_time - last_move_time).count() >= move_delay){
                 // Calculate new head position
                 Position head = snake.front();
                 Position new_head = head;
                 if (dir == LEFT) new_head.col--;
                 else if (dir == RIGHT) new_head.col++;
                 else if (dir == UP) new_head.row--;
                 else if (dir == DOWN) new_head.row++;
                 // Check for collisions
                 if (new_head.row < 0 || new_head.row >= HEIGHT ||
                     new_head.col < 0 || new_head.col >= WIDTH ||
                     (board[new_head.row * WIDTH + new_head.col] == '@' || board[new_head.row * WIDTH + new_head.col] == '=')) {
                     game_over = true;
                     win = false;
                 } else {
                     // Check if food is eaten
                     bool ate_food = false;
                     if (new_head.row == food_pos.row && new_head.col == food_pos.col) {
                         ate_food = true;
                         score++;
                         placeFood(board, snake, food_pos, rng, dist_row, dist_col);
                         food_placed_time = chrono::steady_clock::now();  // Reset food timer
                     }
 
                     // Move snake
                     snake.push_front(new_head);
                     board[new_head.row * WIDTH + new_head.col] = '@';
                     if (!ate_food) {
                         Position tail = snake.back();
                         snake.pop_back();
                         board[tail.row * WIDTH + tail.col] = ' ';
                     }
                     
                     // Check if food should disappear
                     auto current_time = chrono::steady_clock::now();
                     if (chrono::duration_cast<chrono::milliseconds>(current_time - food_placed_time).count() >= food_disappear_time) {
                         board[food_pos.row * WIDTH + food_pos.col] = ' ';
                         placeFood(board, snake, food_pos, rng, dist_row, dist_col);
                         food_placed_time = chrono::steady_clock::now();
                     }
 
                     // Check win condition
                     if (score >= required_score) {
                         game_over = true;
                         win = true;
                     }
                 }
             /*Position head = snake.front();
             Position new_head = head;
             if (dir == LEFT) new_head.col--;
             else if (dir == RIGHT) new_head.col++;
             else if (dir == UP) new_head.row--;
             else if (dir == DOWN) new_head.row++;*/
         
                 // Control game speed
                 this_thread::sleep_for(chrono::milliseconds(move_delay));
             }
         }
 
         #ifndef _WIN32
         resetTerminal();
         #endif
 
         // Handle game end
         cout << "\033[2J\033[H";
         if (win) {
            number_of_key++;
            cout << "You win!" << endl;
            continue_playing = false;
         } else {
             cout << "Game over! Choose an option:" << endl;
             cout << "1. Retry" << endl;
             cout << "2. Save" << endl;
             cout << "3. Quit" << endl;
             char choice;
             cin >> choice;
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
 
             if (choice == '1') {
                 // Retry
             } else if (choice == '2') {
                 ofstream save_file("save.txt");
                 if (save_file.is_open()) {
                     save_file << "Difficulty: " << difficulty << endl;
                     save_file << "Score: " << score << endl;
                     save_file.close();
                     cout << "Game saved." << endl;
                 } else {
                     cout << "Unable to save game." << endl;
                 }
                 continue_playing = false;
             } else if (choice == '3') {
                 continue_playing = false;
             } else {
                 cout << "Invalid choice, quitting." << endl;
                 continue_playing = false;
             }
         }
     }
 
     return;
 }

 const int N = 9;

// 检查位置
bool isSafe(std::vector<std::vector<int>>& board, int row, int col, int num) {
    for (int i = 0; i < N; ++i) {
        if (board[row][i] == num) return false;
    }
    for (int i = 0; i < N; ++i) {
        if (board[i][col] == num) return false;
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i + startRow][j + startCol] == num) return false;
        }
    }

    return true;
}

// 求解数独
bool solveSudoku(std::vector<std::vector<int>>& board, std::mt19937& rng) {
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (board[row][col] == 0) {
                std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                std::shuffle(numbers.begin(), numbers.end(), rng);
                for (int num : numbers) {
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board, rng)) {
                            return true;
                        } else {
                            board[row][col] = 0;
                        }
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// 生成数独题
std::vector<std::vector<int>> generateSudoku(char difficulty) {
    std::vector<std::vector<int>> board(N, std::vector<int>(N, 0));
    std::random_device rd;
    std::mt19937 rng(rd());

    solveSudoku(board, rng);

    // 设置难度
    int numToRemove;
    switch (difficulty_of_the_quanbuyouxi) {
        case '1':
            numToRemove = 10;
            break;
        case '2':
            numToRemove = 20;
            break;
        case '3':
            numToRemove = 30;
            break;
        default:
            numToRemove = 2;  // 默认难度为 normal
            break;
    }

    std::uniform_int_distribution<int> distRow(0, N - 1);
    std::uniform_int_distribution<int> distCol(0, N - 1);

    // 随机移除
    while (numToRemove > 0) {
        int row = distRow(rng);
        int col = distCol(rng);
        if (board[row][col] != 0) {
            board[row][col] = 0;
            --numToRemove;
        }
    }

    return board;
}

// 记录未完成数量
int countRemainingCells(const std::vector<std::vector<int>>& board) {
    int count = 0;
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (board[row][col] == 0) {
                ++count;
            }
        }
    }
    return count;
}

// 打印数独棋盘
void printBoard(const std::vector<std::vector<int>>& board, int remaining, int mistakesLeft) {
    std::cout << "Current Sudoku Puzzle (Remaining cells: " << remaining << ", Mistakes left: " << mistakesLeft << "):" << std::endl;
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            std::cout << board[row][col] << " ";
        }
        std::cout << std::endl;
    }
}

// 检查数独是否已完成
bool isBoardComplete(const std::vector<std::vector<int>>& board) {
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (board[row][col] == 0) {
                return false;
            }
        }
    }
    return true;
}

void shudu() {
    std::cout << "Welcome to Sudoku!" << std::endl;
    std::cout << "How to play:" << std::endl;
    std::cout << "1. Choose a difficulty level: 'e' for easy, 'n' for normal, 'h' for hard." << std::endl;
    std::cout << "2. Enter the row (1 - 9), column (1 - 9), and number (1 - 9) separated by spaces to fill in a cell." << std::endl;
    std::cout << "3. Enter -1 at any time to quit the game." << std::endl;
    std::cout << "4. Each difficulty has a limited number of mistakes allowed (3 for easy, 6 for normal, 9 for hard)." << std::endl;
    std::cout << "Press Enter to start the game..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 提示选择难度
    char difficulty;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 根据难度设置错误保护次数
    int mistakesAllowed;
    switch (difficulty_of_the_quanbuyouxi) {
        case '1':
            mistakesAllowed = 3;
            break;
        case '2':
            mistakesAllowed = 6;
            break;
        case '3':
            mistakesAllowed = 9;
            break;
        default:
            mistakesAllowed = 1000;  // easyeasy!!
            break;
    }
    int mistakesLeft = mistakesAllowed;

    // 生成数独题
    std::vector<std::vector<int>> puzzle = generateSudoku(difficulty);

    int remaining = countRemainingCells(puzzle);
    printBoard(puzzle, remaining, mistakesLeft);

    while (!isBoardComplete(puzzle) && mistakesLeft > 0) {
        int row, col, num;
        std::cout << "Enter row (1 - 9), column (1 - 9), and number (1 - 9) (separated by spaces), or -1 to quit: ";
        std::cin >> row;
        cout << "\033[2J\033[H";
        if (row == -1) {
            std::cout << "Quit the game halfway, Game Over" << std::endl;
            break;
        }
        std::cin >> col >> num;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    
        row--;
        col--;

        if (row >= 0 && row < N && col >= 0 && col < N && num >= 1 && num <= 9) {
            if (isSafe(puzzle, row, col, num)) {
                puzzle[row][col] = num;
                std::cout << "Number placed successfully." << std::endl;
            } else {
                mistakesLeft--;
                if (mistakesLeft > 0) {
                    std::cout << "Invalid move. You have " << mistakesLeft << " mistakes left. Please try again." << std::endl;
                } else {
                    std::cout << "You have used up all your mistake allowances. Game Over." << std::endl;
                    break;
                }
            }
        } else {
            std::cout << "Invalid input. Please try again." << std::endl;
        }

        remaining = countRemainingCells(puzzle);
        printBoard(puzzle, remaining, mistakesLeft);
    }

    if (isBoardComplete(puzzle)) {
        std::cout << "Congratulations! You solved the sudoku." << std::endl;
        number_of_key++;
    } else if (mistakesLeft <= 0) {
        std::cout << "You have used up all your mistake allowances. Game Over." << std::endl;
    }

    return;
}    


const int SIZE = 4; // 游戏区域大小
vector<vector<int>> board(SIZE, vector<int>(SIZE, 0)); // 游戏区域
int targetScore = 0; // 目标分数

// 在随机空位置生成一个 2 或 4
void addRandomTile() {
    vector<pair<int, int>> emptyCells;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                emptyCells.push_back({i, j});
            }
        }
    }
    if (!emptyCells.empty()) {
        int index = rand() % emptyCells.size();
        int value = (rand() % 2 + 1) * 2; // 生成 2 或 4
        board[emptyCells[index].first][emptyCells[index].second] = value;
    }
}

// 初始化游戏
void initGame() {
    srand(static_cast<unsigned int>(time(nullptr)));
    addRandomTile();
    addRandomTile();
}

// 计算当前总分
int calculateTotalScore() {
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            total += board[i][j];
        }
    }
    return total;
}

// 打印游戏区域
void printBoard() {
    system("clear"); // 清屏
    cout << "Welcome to 2048!" << endl;
    cout << "Use W (Up), A (Left), S (Down), D (Right) to move the tiles." << endl;
    cout << "Merge tiles with the same number to reach the target score!" << endl;
    cout << "Target Score: " << targetScore << endl;
    cout << "Current Total: " << calculateTotalScore() << endl;
    cout << "Remaining: " << targetScore - calculateTotalScore() << endl;
    cout << "-----------------------------" << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                cout << " . ";
            } else {
                cout << " " << board[i][j] << " ";
            }
        }
        cout << endl;
    }
}

// 检查游戏是否结束
bool isGameOver() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                return false;
            }
            if (i < SIZE - 1 && board[i][j] == board[i + 1][j]) {
                return false;
            }
            if (j < SIZE - 1 && board[i][j] == board[i][j + 1]) {
                return false;
            }
        }
    }
    return true;
}

// 向左移动并合并
void moveLeft() {
    for (int i = 0; i < SIZE; i++) {
        vector<int> row = board[i];
        vector<int> newRow(SIZE, 0);
        int index = 0;
        for (int j = 0; j < SIZE; j++) {
            if (row[j] != 0) {
                if (newRow[index] == 0) {
                    newRow[index] = row[j];
                } else if (newRow[index] == row[j]) {
                    newRow[index] *= 2;
                    index++;
                } else {
                    index++;
                    newRow[index] = row[j];
                }
            }
        }
        board[i] = newRow;
    }
}

// 向右移动并合并
void moveRight() {
    for (int i = 0; i < SIZE; i++) {
        vector<int> row = board[i];
        vector<int> newRow(SIZE, 0);
        int index = SIZE - 1;
        for (int j = SIZE - 1; j >= 0; j--) {
            if (row[j] != 0) {
                if (newRow[index] == 0) {
                    newRow[index] = row[j];
                } else if (newRow[index] == row[j]) {
                    newRow[index] *= 2;
                    index--;
                } else {
                    index--;
                    newRow[index] = row[j];
                }
            }
        }
        board[i] = newRow;
    }
}

// 向上移动并合并
void moveUp() {
    for (int j = 0; j < SIZE; j++) {
        vector<int> column(SIZE, 0);
        int index = 0;
        for (int i = 0; i < SIZE; i++) {
            if (board[i][j] != 0) {
                if (column[index] == 0) {
                    column[index] = board[i][j];
                } else if (column[index] == board[i][j]) {
                    column[index] *= 2;
                    index++;
                } else {
                    index++;
                    column[index] = board[i][j];
                }
            }
        }
        for (int i = 0; i < SIZE; i++) {
            board[i][j] = column[i];
        }
    }
}

// 向下移动并合并
void moveDown() {
    for (int j = 0; j < SIZE; j++) {
        vector<int> column(SIZE, 0);
        int index = SIZE - 1;
        for (int i = SIZE - 1; i >= 0; i--) {
            if (board[i][j] != 0) {
                if (column[index] == 0) {
                    column[index] = board[i][j];
                } else if (column[index] == board[i][j]) {
                    column[index] *= 2;
                    index--;
                } else {
                    index--;
                    column[index] = board[i][j];
                }
            }
        }
        for (int i = 0; i < SIZE; i++) {
            board[i][j] = column[i];
        }
    }
}

// 处理用户输入
void handleInput() {
    char input;
    cin >> input;
    switch (input) {
        case 'a': moveLeft(); break;
        case 'd': moveRight(); break;
        case 'w': moveUp(); break;
        case 's': moveDown(); break;
        default: return;
    }
    addRandomTile();
}

// 游戏介绍和难度选择！！！！！！！！！！！！！！！！！！！！！
void showWelcomeScreen() {
    system("clear");
    cout << "Welcome to 2048!" << endl;
    cout << "Use W (Up), A (Left), S (Down), D (Right) to move the tiles." << endl;
    cout << "Merge tiles with the same number to reach the target score!" << endl;
    cout << "Press any key to continue..." << endl;
    cin.get(); // 等待用户按任意键
    system("clear");
    switch (difficulty_of_the_quanbuyouxi) {
        case '1': targetScore = 300; break;
        case '2': targetScore = 500; break;
        case '3': targetScore = 700; break;
        default: targetScore = 500; break; // 默认普通难度
    }
}

void game2048() {
    showWelcomeScreen();
    initGame();
    while (true) {
        printBoard();
        if (isGameOver()) {
            cout << "Game Over! You did not reach the target score." << endl;
            break;
        }
        if (calculateTotalScore() >= targetScore) {
            cout << "Congratulations! You reached the target score!" << endl;
            number_of_key++;
            break;
        }
        handleInput();
    }
    return;
}



vector <int> already_played;

vector <void(*)()> games = {AirplaneBattleGame, snake, shudu, game2048};
int startSmallGame(){
    uniform_int_distribution<> dis(0, games.size()-1);
    int a = dis(gen);
    if (already_played.size() == games.size()){
        already_played.clear();
    }
    if (find(already_played.begin(), already_played.end(), a) == already_played.end()){
        already_played.push_back(a);
        games[a]();
    }
    else{
        startSmallGame();
    }
    return a;   
}
