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

#include <iostream>
#include <vector>
#include <deque>
#include <chrono>
#include <thread>
#include <random>
#include <ctime>
#include <limits>
#include <ios>
#include <fstream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#endif

using namespace std;

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
int main() {
    // Display game instructions and wait for confirmation
    cout << "Welcome to the Snake Game!\n";
    cout << "Rules:\n";
    cout << "- Use W/A/S/D keys to move the snake up/left/down/right.\n";
    cout << "- Eating food (*) will increase length and score.\n";
    cout << "- Avoid hitting walls or your own body.\n";
    cout << "- Reach the target score based on difficulty to win.\n";
    cout << "Press Enter to continue...\n";
    string dummy;
    getline(cin, dummy);

    // Prompt for difficulty selection
    cout << "Choose difficulty: (e)asy, (n)ormal, (h)ard" << endl;
    char difficulty;
    cin >> difficulty;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Set target score, movement delay, and food disappearance time
    int required_score;
    int move_delay;
    int food_disappear_time;
    switch (difficulty) {
        case 'e':
            required_score = 3;
            move_delay = 1000;  // 1 second
            food_disappear_time = 25000;  // 25 seconds
            break;
        case 'n':
            required_score = 5;
            move_delay = 700;  // 0.7 seconds
            food_disappear_time = 18000;  // 18 seconds
            break;
        case 'h':
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

    return 0;
}
