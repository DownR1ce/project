#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <algorithm>

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

    // 难度
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

    std::uniform_int_distribution<int> distRow(0, N - 1);
    std::uniform_int_distribution<int> distCol(0, N - 1);

    // 随机移除数字
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

// 记录没写完的
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

// 检查数独是否完成
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

    // 根据难度设置错误保护次数
    int mistakesAllowed;
    switch (difficulty) {
        case 'e':
            mistakesAllowed = 3;
            break;
        case 'n':
            mistakesAllowed = 6;
            break;
        case 'h':
            mistakesAllowed = 9;
            break;
        default:
            mistakesAllowed = 6;  // 默认难度为 normal
            break;
    }
    int mistakesLeft = mistakesAllowed;

    // 生成题
    std::vector<std::vector<int>> puzzle = generateSudoku(difficulty);

    int remaining = countRemainingCells(puzzle);
    printBoard(puzzle, remaining, mistakesLeft);

    while (!isBoardComplete(puzzle) && mistakesLeft > 0) {
        int row, col, num;
        std::cout << "Enter row (1 - 9), column (1 - 9), and number (1 - 9) (separated by spaces), or -1 to quit: ";
        std::cin >> row;
        if (row == -1) {
            std::cout << "游戏失败" << std::endl;
            break;
        }
        std::cin >> col >> num;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空输入缓冲区

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
                    std::cout << "You have used up all your mistake allowances. Game ended." << std::endl;
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
    } else if (mistakesLeft <= 0) {
        std::cout << "You have used up all your mistake allowances. Game ended." << std::endl;
    }

    return 0;
}
