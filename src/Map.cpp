#include "Map.hpp"
#include "App.hpp"
#include "../PTSD/include/Util/Logger.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

// 定義靜態成員變數
const std::string Map::ORANGE = "\033[33m";
const std::string Map::WHITE = "\033[37m";
const std::string Map::RESET = "\033[0m";

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

    // 設置一些顏色用於不同的地圖元素
    const std::string BLOCK = "█";   // 實心方塊
    const std::string EMPTY = "·";   // 空白區域

    // 輸出頂部邊框
    LOG_INFO("┌──────────────────────────────────┐");

    for (size_t i = 0; i < matrix.size(); ++i) {
        const auto& row = matrix[i];
        std::string line = "│ ";

        for (int cell : row) {
            line += (cell == 1) ? ORANGE + BLOCK + RESET : WHITE + EMPTY + RESET;
        }

        line += " │";
        LOG_INFO("{}", line);
    }
    // 輸出底部邊框
    LOG_INFO("└──────────────────────────────────┘");

    // 額外輸出地圖資訊
    // LOG_INFO("Map Details:");
    // LOG_INFO("- Total Rows: {}", matrix.size());
    // LOG_INFO("- Total Columns: {}", matrix[0].size());
    // LOG_INFO("- Filled Blocks: {}", CountFilledBlocks(matrix));
}

// 輔助函數：計算實心方塊數量
int Map::CountFilledBlocks(const std::vector<std::vector<int>>& matrix) {
    int count = 0;
    for (const auto& row : matrix) {
        count += std::count(row.begin(), row.end(), 1);
    }
    return count;
}


