#include "Map.hpp"
#include "App.hpp"
#include "../PTSD/include/Util/Logger.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


std::vector<std::vector<int>> Map::LoadMap(const std::string& filename) {
    std::vector<std::vector<int>> matrix;
    std::ifstream file(filename);

    if (!file.is_open()) {
        LOG_ERROR("Failed to open map file: {}", filename);
        return matrix;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        if (!row.empty()) {
            matrix.push_back(row);
        }
    }

    LOG_INFO("Loaded map with {} rows", matrix.size());
    return matrix;
}


void Map::RenderMap(const std::vector<std::vector<int>>& matrix){
    LOG_INFO("Rendering map to console:");

}

// 輔助函數：計算實心方塊數量
int Map::CountFilledBlocks(const std::vector<std::vector<int>>& matrix) {
    int count = 0;
    for (const auto& row : matrix) {
        count += std::count(row.begin(), row.end(), 1);
    }
    return count;
}

// 在 AppUtil.cpp 中添加
void App::CreateMapTiles(const std::vector<std::vector<int>>& map){
    // 先清除現有的地圖磚塊
    m_MapTiles.clear();

    // 檢查地圖是否為空
    if (map.empty()){
        LOG_ERROR("Map is empty, cannot create tiles?");
        return;
    }

    LOG_DEBUG("Creating Map Tiles - Total Map Rows: {}", map.size());

    // 增大磚塊尺寸
    float tileSize = 55.0f;
    float startX = -387.0f; // 調整起始位置，使地圖更居中
    float startY = 223.0f;  // 調整起始位置，使地圖更靠上

    int tilesCreated = 0;

    std::string tileImagePath = GA_RESOURCE_DIR"/Image/Character/orange_tile.png";
    for (size_t y = 0; y < map.size(); ++y) {

        // 計算每行最多顯示的列數
        // size_t maxColumns = std::min(map[y].size(), static_cast<size_t>(16));

        for (size_t x = 0; x < map.size(); ++x) {
            if (map[y][x] == 1) {
                // 創建磚塊
                auto tile = std::make_shared<Character>(tileImagePath);
                float posX = startX + x * tileSize;
                float posY = startY - y * tileSize;

                tile->SetPosition({posX, posY});
                tile->SetZIndex(5); // 確保磚塊在背景之上，但在角色之下
                tile->SetVisible(true);


                m_Root.AddChild(tile);
                m_MapTiles.push_back(tile);
                tilesCreated++;
            }
        }
    }

    LOG_INFO("Total Tiles Created: {}", tilesCreated);
}
