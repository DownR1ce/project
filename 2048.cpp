#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // 用于 usleep 函数

using namespace std;

// 游戏界面
const int SIZE = 4;
vector<vector<int>> board(SIZE, vector<int>(SIZE, 0));
int targetScore = 0; 

// 在随机空位置生成 2 或 4
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
        int value = (rand() % 2 + 1) * 2;
        board[emptyCells[index].first][emptyCells[index].second] = value;
    }
}

// 初始化游戏
void initGame() {
    srand(static_cast<unsigned int>(time(nullptr)));
    addRandomTile();
    addRandomTile();
}

// 打印游戏区域
void printBoard() {
    system("clear");
    cout << "Welcome to 2048!" << endl;
    cout << "Use W (Up), A (Left), S (Down), D (Right) to move the tiles." << endl;
    cout << "Merge tiles with the same number to reach the target score!" << endl;
    cout << "Target Score: " << targetScore << endl;
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

// 处理输入
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

// 游戏介绍和难度选择！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
void showWelcomeScreen() {
    system("clear");
    cout << "Welcome to 2048!" << endl;
    cout << "Use W (Up), A (Left), S (Down), D (Right) to move the tiles." << endl;
    cout << "Merge tiles with the same number to reach the target score!" << endl;
    cout << "Press any key to continue..." << endl;
    cin.get(); // 等待用户按任意键
    system("clear");
    cout << "Select Difficulty:" << endl;
    cout << "1. Easy (Target Score: 300)" << endl;
    cout << "2. Normal (Target Score: 500)" << endl;
    cout << "3. Hard (Target Score: 700)" << endl;
    int choice;
    cin >> choice;
    switch (choice) {
        case 1: targetScore = 300; break;
        case 2: targetScore = 500; break;
        case 3: targetScore = 700; break;
        default: targetScore = 500; break;
    }
}

int main() {
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
            break;
        }
        handleInput();
    }
    return 0;
}
