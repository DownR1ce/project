#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

const int N = 9;

// 检查在 (row, col) 位置放置数字 num 是否合法
bool isValid(std::vector<std::vector<int>>& board, int row, int col, int num) {
    // 检查行
    for (int i = 0; i < N; ++i) {
        if (board[row][i] == num) {
            return false;
        }
    }
    // 检查列
    for (int i = 0; i < N; ++i) {
        if (board[i][col] == num) {
            return false;
        }
    }
    // 检查 3x3 子网格
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }
    return true;
}

// 使用回溯法求解数独
bool solveSudoku(std::vector<std::vector<int>>& board) {
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (board[row][col] == 0) {
                std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));
                for (int num : numbers) {
                    if (isValid(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board)) {
                            return true;
                        }
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// 生成数独谜题
std::vector<std::vector<int>> generateSudoku() {
    std::vector<std::vector<int>> board(N, std::vector<int>(N, 0));
    solveSudoku(board);
    // 挖空棋盘
    int emptyCells = rand() % 11 + 40;  // 可根据难度调整挖空数量
    while (emptyCells > 0) {
        int row = rand() % N;
        int col = rand() % N;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            --emptyCells;
        }
    }
    return board;
}

// 打印数独棋盘
void printBoard(const std::vector<std::vector<int>>& board) {
    for (const auto& row : board) {
        for (int num : row) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    srand(time(0));
    std::vector<std::vector<int>> sudokuPuzzle = generateSudoku();
    printBoard(sudokuPuzzle);
    return 0;
}   
