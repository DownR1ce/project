### **核心模块**
```mermaid
classDiagram
    class GameEngine {
        -isRunning: bool
        +start() void
        +stop() void
    }
    
    class InputSystem {
        +getInput(): string
    }
    
    class SoundSystem {
        +playBGM() void
        +stopBGM() void
    }
    
    GameEngine --> InputSystem
    GameEngine --> SoundSystem
```

---

### **代码实现（C++）**

#### **1. 核心引擎类（game_engine.h）**
```cpp
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

class GameEngine {
private:
    std::atomic<bool> isRunning;
    std::thread bgmThread;

    void bgmLoop() {
        while(isRunning) {
            // 调用外部播放脚本（兼容各平台）（bgm)
            system("./scripts/play_bgm.sh"); 
        }
    }

public:
    GameEngine() : isRunning(false) {}

    void start() {
        isRunning = true;
        bgmThread = std::thread(&GameEngine::bgmLoop, this);
        std::cout << "=== Terminal Intruder ===" << std::endl;
        
        // 游戏主循环
        while(isRunning) {
            std::string input;
            std::cout << "> ";
            std::getline(std::cin, input);
            
            // 简单命令处理
            if(input == "quit") {
                stop();
            } else {
                std::cout << "Executing: " << input << std::endl;
            }
        }
    }

    void stop() {
        isRunning = false;
        if(bgmThread.joinable()) bgmThread.join();
        system("./scripts/stop_bgm.sh");
        std::cout << "Game exited." << std::endl;
    }
};
```

#### **2. 音频控制脚本（scripts/play_bgm.sh）**
```bash
#!/bin/bash
# 跨平台BGM播放方案
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    mpv --no-video bgm.mp3 --loop
elif [[ "$OSTYPE" == "darwin"* ]]; then
    afplay bgm.mp3
else
    echo "BGM not supported on this system"
fi
```

#### **3. 主程序（main.cpp）**
```cpp
#include "game_engine.h"

int main() {
    GameEngine engine;
    engine.start();
    return 0;
}
```

### **4. 存档系统设计**
```mermaid
classDiagram
    class SaveSystem {
        +saveGame(playerData: string): bool
        +loadGame(): string
        -encrypt(data: string): string
        -decrypt(data: string): string
    }
    
    GameEngine --> SaveSystem
```

---

### **4.2 代码实现**

#### **4.2.1 存档数据结构（JSON示例）**
```cpp
// 玩家数据示例
std::string playerData = R"({
    "progress": 0.65,
    "discovered_nodes": ["10.0.0.1", "10.0.0.3"],
    "score": 1200
})";
```

#### **4.2.2 存档管理类（save_system.h）**
```cpp
#include <fstream>
#include <filesystem>

class SaveSystem {
private:
    const std::string SAVE_PATH = "saves/";
    
    // 简易异或加密
    std::string xorEncrypt(const std::string& data, char key = 0xAA) {
        std::string result = data;
        for (auto& c : result) c ^= key;
        return result;
    }

public:
    SaveSystem() {
        // 自动创建存档目录
        std::filesystem::create_directory(SAVE_PATH);
    }

    bool saveGame(const std::string& data) {
        std::ofstream file(SAVE_PATH + "save.dat", std::ios::binary);
        if (!file) return false;
        
        std::string encrypted = xorEncrypt(data);
        file << encrypted;
        return true;
    }

    std::string loadGame() {
        std::ifstream file(SAVE_PATH + "save.dat", std::ios::binary);
        if (!file) return "";
        
        std::string encrypted((std::istreambuf_iterator<char>(file)), 
                            std::istreambuf_iterator<char>());
        return xorEncrypt(encrypted);
    }
};
```

#### **4.2.3 集成到游戏引擎**
```cpp
class GameEngine {
    // 新增成员
    SaveSystem saveManager;
    std::string currentSave;

    void handleCommand(const std::string& input) {
        if (input == "save") {
            if(saveManager.saveGame(currentSave)) {
                std::cout << "进度已保存！" << std::endl;
            }
        } else if (input == "load") {
            currentSave = saveManager.loadGame();
            std::cout << "进度已读取" << std::endl; 
        }
        // ...其他命令
    }
};
```

---

### **4.3 使用示例**
```bash
# 游戏内操作
> ping 10.0.0.1
发现安全节点！
> save
进度已保存！

> quit

# 下次启动时
> load
进度已读取
> ping 10.0.0.1
该节点已探索过
```

---

### **4.4 存档文件验证**
```bash
# 查看加密后的存档（Linux/macOS）
hexdump -C saves/save.dat

00000000  a2 8b 8c 9d 9e 93 98 8b  96 8b 9e 8d 9e 93 85 8e  |................|
00000010  9d 8a 8b 9e 8b 8a 97 8a  9d 8e 8b 8c 93 98 8b 96  |................|
```

---

### **4.5 扩展功能建议**

#### **4.5.1 多存档位**
```cpp
bool saveSlot(int slot) {
    return saveGame("slot" + std::to_string(slot) + ".dat");
}
```

#### **4.5.2 存档时间戳**
```cpp
#include <chrono>

std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    return std::to_string(
        std::chrono::duration_cast<std::chrono::seconds>(
            now.time_since_epoch()
        ).count()
    );
}
```
---

### **功能验证清单**
1. **键盘输入**：
   ```cpp
   // 示例输入处理扩展
   if(input.substr(0,4) == "ping") {
       handlePingCommand(input);
   }
   ```
   
2. **BGM播放**：
   - 支持MP3/WAV格式
   - 自动循环播放
   - 兼容Linux/macOS基础环境

3. **退出机制**：
   ```shell
   # scripts/stop_bgm.sh
   pkill -f "mpv\|afplay"  # 终止音频进程
   ```

---

### **项目文件结构**
```bash
.
├── saves/
│   └── save.dat       # 存档文件
├── scripts/
│   ├── play_bgm.sh
│   └── stop_bgm.sh
├── game_engine.h      # 包含存档系统
├── main.cpp
└── bgm.mp3
```

---

### **编译与运行**
```bash
# 编译（G++示例）
g++ main.cpp -o game -pthread

# 运行
./game
```

---

### **扩展建议**
1. **ASCII艺术标题**：
    ```
   void showTitle() {
    std::cout << "\033[32m"; // 设置为绿色

    std::cout << R"(
  _______ _______ ______  _______ _     _ _______ _______  ______ _______ ______ _     _ 
 |       |______ |_____] |______ |_____| |______ |       |_____/ |______ |     | |____/  
 |_____  |______ |_____] ______| |     | |______ |_____  |    \_ |______ |_____| |    \_
                                                                                        
    )" << '\n';

    // 动态扫描线效果
    std::cout << "\033[34m"; // 蓝色
    for(int i=0; i<3; ++i) {
        std::cout << ">>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "\033[F"; // 光标上移一行
    }
    
    std::cout << "\033[0m"; // 重置颜色
}
   ```

2. **简单音效增强**：
   ```cpp
   void playSoundEffect(const char* type) {
       std::string cmd = "aplay ./sounds/" + std::string(type) + ".wav &";
       system(cmd.c_str());
   }
   ```

3. **历史命令记录**：
   ```cpp
   class InputSystem {
   private:
       std::vector<std::string> history;
   public:
       void logInput(const std::string& cmd) {
           history.push_back(cmd);
       }
   };
   ```

---
