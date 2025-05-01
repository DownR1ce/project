#include <iostream>
#include <vector>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

using namespace std;

// 获取终端尺寸
pair<int, int> getTerminalSize() {
    int width = 130;  // 默认值
    int height = 50; // 默认值
    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    #else
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0) {
        width = size.ws_col;
        height = size.ws_row;
    }
    #endif
    
    return make_pair(width, height);
}

// 居中显示多行文本
void printCentered(const vector<string>& lines) {
    pair<int, int> terminalSize = getTerminalSize();
    int termWidth = terminalSize.first;
    int termHeight = (terminalSize.second);
    
    // 计算垂直方向的空行数
    int verticalPadding = ((termHeight - lines.size()) / 2);
    verticalPadding = (max(0, verticalPadding));
    // 打印上方空行
    for (int i = 0; i < verticalPadding-10; ++i) {
        cout << endl;
    }
    
    // 打印每行文本（水平居中）

    for (const auto& line : lines) {
        int horizontalPadding = (termWidth - line.length()) / 2;
        horizontalPadding = max(0, horizontalPadding);
        
        cout << string(horizontalPadding, ' ') << line << endl;
    }
}

void printFor_digitNumber(const vector<string>& lines) {
    pair<int, int> terminalSize = getTerminalSize();
    int termWidth = terminalSize.first;
    int termHeight = terminalSize.second;

    int verticalPadding = ((termHeight - lines.size()) / 2);
    verticalPadding = (max(0, verticalPadding));
    // 打印上方空行
    for (int i = 0; i < verticalPadding-12; ++i) {
        cout << endl;
    }

    // 水平居中

    for (const auto& line : lines) {
        int horizontalPadding = (((termWidth - line.length()) / 2)+47);
        horizontalPadding = max(0, horizontalPadding);
        
        cout << string(horizontalPadding, ' ') << line << endl;
    }
}

void Print_Number (int number){
    vector<vector<string>> digits = {
        //0
        {
            " *** ",
            "*   *",
            "*   *",
            "*   *",
            " *** "
        },
        //1
        {
            "  *  ",
            " **  ",
            "  *  ",
            "  *  ",
            " *** "
        },
        //2
        {
            " *** ",
            "    *",
            "  ** ",
            " *   ",
            "*****"
        },
        //3
        {
            "**** ",
            "    *",
            " *** ",
            "    *",
            "**** "
        },
        //4
        {
            "*   *",
            "*   *",
            "*****",
            "    *",
            "    *"
        },
        //5
        {
            "*****",
            "*    ",
            "**** ",
            "    *",
            "**** "
        },
        //6
        {
            " *** ",
            "*    ",
            "**** ",
            "*   *",
            " *** "
        },
        //7
        {
            "*****",
            "    *",
            "   * ",
            "  *  ",
            " *   "
        },
        //8
        {
            " *** ",
            "*   *",
            " *** ",
            "*   *",
            " *** "
        },
        //9
        {
            " *** ",
            "*   *",
            " ****",
            "    *",
            " *** "
        },
        //10
        {
            "  *  *** ",
            " ** *   *",
            "  * *   *",
            "  * *   *",
            " *** *** "
        },
        //11
        {
            "  *   * ",
            " **  ** ",
            "  *   * ",
            "  *   * ",
            " *** ***"
        },
        //12
        {
            "  *  *** ",
            " **     *",
            "  *   ** ",
            "  *  *   ",
            " *** ****"
        },
        //13
        {
            "  * **** ",
            " **     *",
            "  *  *** ",
            "  *     *",
            " *** ****"
        },
        //14
        {
            "  * *   *",
            " ** *   *",
            "  * *****",
            "  *     *",
            " ***     *"
        }
    };
    digits[number].emplace(digits[number].begin(), "   Number of 5 * 5 bombs around the player(x*y):");
    printFor_digitNumber(digits[number]);
}
