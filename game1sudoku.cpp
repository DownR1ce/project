#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

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

// 打印数独棋盘（增量更新版本）
void updateBoard(const std::vector<std::vector<int>>& currentBoard, 
                 std::vector<std::vector<int>>& prevBoard) {
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (currentBoard[row][col] != prevBoard[row][col]) {
                // 移动到对应的位置并更新数字
                std::cout << "\033[" << row + 1 << ";" << col * 2 + 1 << "H" << currentBoard[row][col];
                prevBoard[row][col] = currentBoard[row][col];
            }
        }
    }
    std::cout.flush(); // 确保立即显示更新
}

int main() {
    // 提示选择难度
    std::cout << "Choose difficulty: (e)asy, (n)ormal, (h)ard" << std::endl;
    char difficulty;
    std::cin >> difficulty;

    // 生成一个数独谜题
    std::vector<std::vector<int>> puzzle = generateSudoku(difficulty);
    std::vector<std::vector<int>> prevPuzzle = puzzle;

    std::cout << "Generated Sudoku Puzzle:" << std::endl;
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            std::cout << puzzle[row][col] << " ";
        }
        std::cout << std::endl;
    }

    // 求解数独谜题
    std::vector<std::vector<int>> solution = puzzle;
    if (solveSudoku(solution)) {
        std::cout << "\nSolved Sudoku:" << std::endl;
        updateBoard(solution, prevPuzzle);
    } else {
        std::cout << "\nNo solution exists." << std::endl;
    }

    return 0;
}
