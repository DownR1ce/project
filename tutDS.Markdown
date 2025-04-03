---

### **1. 存档系统设计**
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

### **2. 代码实现**

#### **2.1 存档数据结构（JSON示例）**
```cpp
// 玩家数据示例
std::string playerData = R"({
    "progress": 0.65,
    "discovered_nodes": ["10.0.0.1", "10.0.0.3"],
    "score": 1200
})";
```

#### **2.2 存档管理类（save_system.h）**
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

#### **2.3 集成到游戏引擎**
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

### **3. 使用示例**
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

### **4. 存档文件验证**
```bash
# 查看加密后的存档（Linux/macOS）
hexdump -C saves/save.dat

00000000  a2 8b 8c 9d 9e 93 98 8b  96 8b 9e 8d 9e 93 85 8e  |................|
00000010  9d 8a 8b 9e 8b 8a 97 8a  9d 8e 8b 8c 93 98 8b 96  |................|
```

---

### **5. 扩展功能建议**

#### **5.1 多存档位**
```cpp
bool saveSlot(int slot) {
    return saveGame("slot" + std::to_string(slot) + ".dat");
}
```

#### **5.2 存档时间戳**
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

### **6. 教学重点**
1. **文件操作**：`<fstream>`库的使用
2. **目录管理**：C++17的`<filesystem>`功能
3. **数据加密**：基础的异或加密原理
4. **异常处理**：文件访问的失败检测
5. **数据序列化**：简单JSON格式处理

---

### **7. 项目结构更新**
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
