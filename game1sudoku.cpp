#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

const int N = 9;

// 检查在 (row, col) 位置放置 num 是否合法
bool isSafe(std::vector<std::vector<int>>& board, int row, int col, int num) {
    // 检查行
    for (int i = 0; i < N; ++i) {
        if (board[row][i] == num) return false;
    }

    // 检查列
    for (int i = 0; i < N; ++i) {
        if (board[i][col] == num) return false;
    }

    // 检查 3x3 子网格
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
bool solveSudoku(std::vector<std::vector<int>>& board) {
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= 9; ++num) {
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board)) {
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

// 生成一个数独谜题
std::vector<std::vector<int>> generateSudoku(char difficulty) {
    std::vector<std::vector<int>> board(N, std::vector<int>(N, 0));
    solveSudoku(board);

    // 根据难度设置移除数字的数量
    int numToRemove;
    switch (difficulty) {
        case 'e':
            numToRemove = 10;
            break;
        case 'n':
            numToRemove = 20;
            break;
        case 'h':
            numToRemove = 30;
            break;
        default:
            numToRemove = 20;  // 默认难度为 normal
            break;
    }

    // 随机移除一些数字来创建谜题
    std::srand(std::time(0));
    while (numToRemove > 0) {
        int row = std::rand() % N;
        int col = std::rand() % N;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            --numToRemove;
        }
    }

    return board;
}

// 打印数独棋盘
void printBoard(const std::vector<std::vector<int>>& board) {
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

int main() {
    // 提示选择难度
    std::cout << "Choose difficulty: (e)asy, (n)ormal, (h)ard" << std::endl;
    char difficulty;
    std::cin >> difficulty;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空输入缓冲区

    // 生成一个数独谜题
    std::vector<std::vector<int>> puzzle = generateSudoku(difficulty);

    std::cout << "Generated Sudoku Puzzle:" << std::endl;
    printBoard(puzzle);

    while (!isBoardComplete(puzzle)) {
        int row, col, num;
        std::cout << "Enter row (1 - 9), column (1 - 9), and number (1 - 9) (separated by spaces), or -1 to quit: ";
        std::cin >> row;
        if (row == -1) {
            break;
        }
        std::cin >> col >> num;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空输入缓冲区

        // 调整为 0 索引
        row--;
        col--;

        if (row >= 0 && row < N && col >= 0 && col < N && num >= 1 && num <= 9) {
            if (isSafe(puzzle, row, col, num)) {
                puzzle[row][col] = num;
                std::cout << "Number placed successfully." << std::endl;
            } else {
                std::cout << "Invalid move. Please try again." << std::endl;
            }
        } else {
            std::cout << "Invalid input. Please try again." << std::endl;
        }

        printBoard(puzzle);
    }

    if (isBoardComplete(puzzle)) {
        std::cout << "Congratulations! You solved the sudoku." << std::endl;
    } else {
        std::cout << "Game ended." << std::endl;
    }

    return 0;
}
