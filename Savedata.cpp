#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "player.h"

struct GameSaveData {
    int player_heart;
    std::vector<std::string> player_inventory;
    int number_of_mineSweeping;
    int number_of_key;
    int XuYaoDe_number_of_key;
    coordinate Player_coordinate;
    int number_of_mine;
    int x;
    int y;
    char difficulty_of_the_quanbuyouxi;

    std::vector<std::vector<char>> MazeMap_hide;
    std::vector<std::vector<char>> MazeMap_show;
};

void saveMap(std::ofstream& out, const std::vector<std::vector<char>>& map) {
    size_t rows = map.size();
    out.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
    
    for (const auto& row : map) {
        size_t cols = row.size();
        out.write(reinterpret_cast<const char*>(&cols), sizeof(cols));
        out.write(reinterpret_cast<const char*>(row.data()), cols * sizeof(char));
    }
}

bool loadMap(std::ifstream& in, std::vector<std::vector<char>>& map) {
    size_t rows;
    in.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    map.resize(rows);
    
    for (auto& row : map) {
        size_t cols;
        in.read(reinterpret_cast<char*>(&cols), sizeof(cols));
        row.resize(cols);
        in.read(reinterpret_cast<char*>(row.data()), cols * sizeof(char));
    }
    
    return true;
}


bool loadGame(GameSaveData& data, const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Unable to read archive file: " << filename << std::endl;
        return false;
    }
    
    // 读取玩家数据
    in.read(reinterpret_cast<char*>(&data.player_heart), sizeof(data.player_heart));
    
    // 读取物品栏
    size_t inventorySize;
    in.read(reinterpret_cast<char*>(&inventorySize), sizeof(inventorySize));
    data.player_inventory.resize(inventorySize);
    for (auto& item : data.player_inventory) {
        size_t itemSize;
        in.read(reinterpret_cast<char*>(&itemSize), sizeof(itemSize));
        item.resize(itemSize);
        in.read(&item[0], itemSize);
    }
    
    in.read(reinterpret_cast<char*>(&data.number_of_mineSweeping), sizeof(data.number_of_mineSweeping));
    in.read(reinterpret_cast<char*>(&data.number_of_key), sizeof(data.number_of_key));
    in.read(reinterpret_cast<char*>(&data.XuYaoDe_number_of_key), sizeof(data.XuYaoDe_number_of_key));
    in.read(reinterpret_cast<char*>(&data.Player_coordinate.x), sizeof(data.Player_coordinate.x));
    in.read(reinterpret_cast<char*>(&data.Player_coordinate.y), sizeof(data.Player_coordinate.y));
    in.read(reinterpret_cast<char*>(&data.number_of_mine), sizeof(data.number_of_mine));
    in.read(reinterpret_cast<char*>(&data.x), sizeof(data.x));
    in.read(reinterpret_cast<char*>(&data.y), sizeof(data.y));
    in.read(reinterpret_cast<char*>(&data.difficulty_of_the_quanbuyouxi), sizeof(data.difficulty_of_the_quanbuyouxi));
    
    // 读取隐藏地图
    if (!loadMap(in, data.MazeMap_hide)) {
        return false;
    }
    
    // 读取显示地图
    if (!loadMap(in, data.MazeMap_show)) {
        return false;
    }
    
    in.close();
    std::cout << "从 " << filename << " Read the archive successfully." << std::endl;
    std::cout << data.player_heart << "\n"<<data.number_of_key<<"\n" << data.number_of_mineSweeping << "\n" << data.XuYaoDe_number_of_key << "\n" << Player_coordinate.x << Player_coordinate.y << "\n" 
    << data.number_of_mine<<data.difficulty_of_the_quanbuyouxi<<std::endl;
    for(int i = 0; i < data.MazeMap_show.size(); i++){
        for(int j = 0; j < data.MazeMap_show[0].size(); j++){
            std::cout<<data.MazeMap_show[i][j];
        }
        std::cout<<std::endl;
    }
    return true;
}

void saveGame(const GameSaveData& data, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Cannot save data: " << filename << std::endl;
        return;
    }
    
    // 保存玩家数据
    out.write(reinterpret_cast<const char*>(&data.player_heart), sizeof(data.player_heart));
    
    // 保存物品栏
    size_t inventorySize = data.player_inventory.size();
    out.write(reinterpret_cast<const char*>(&inventorySize), sizeof(inventorySize));
    for (const auto& item : data.player_inventory) {
        size_t itemSize = item.size();
        out.write(reinterpret_cast<const char*>(&itemSize), sizeof(itemSize));
        out.write(item.c_str(), itemSize);
    }
    
    out.write(reinterpret_cast<const char*>(&data.number_of_mineSweeping), sizeof(data.number_of_mineSweeping));
    out.write(reinterpret_cast<const char*>(&data.number_of_key), sizeof(data.number_of_key));
    out.write(reinterpret_cast<const char*>(&data.XuYaoDe_number_of_key), sizeof(data.XuYaoDe_number_of_key));
    out.write(reinterpret_cast<const char*>(&data.Player_coordinate.x), sizeof(data.Player_coordinate.x));
    out.write(reinterpret_cast<const char*>(&data.Player_coordinate.y), sizeof(data.Player_coordinate.y));
    out.write(reinterpret_cast<const char*>(&data.number_of_mine), sizeof(data.number_of_mine));
    out.write(reinterpret_cast<const char*>(&data.x), sizeof(data.x));
    out.write(reinterpret_cast<const char*>(&data.y), sizeof(data.y));
    out.write(reinterpret_cast<const char*>(&data.difficulty_of_the_quanbuyouxi), sizeof(data.difficulty_of_the_quanbuyouxi));
    
    // 保存隐藏地图
    saveMap(out, data.MazeMap_hide);
    
    // 保存显示地图
    saveMap(out, data.MazeMap_show);
    
    out.close();
    std::cout << "Game has been saved " << filename << std::endl;
}
