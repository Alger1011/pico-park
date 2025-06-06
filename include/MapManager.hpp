#ifndef MAP_MANAGER_HPP
#define MAP_MANAGER_HPP

#include <vector>
#include <memory>
#include "Character.hpp"
#include "Util/Renderer.hpp"
#include <glm/glm.hpp>  // 確保包含 glm 頭文件

class Camera;

class MapManager {
public:
    MapManager(Util::Renderer& root);

    // 從檔案載入地圖
    void LoadMap(const std::string& filename);

    // 根據相機位置更新地圖磚塊的可見性
    // void UpdateVisibility(const Camera& camera);
    //
    // // 檢查指定位置是否有磚塊
    // bool HasTileAt(const glm::vec2& position) const;
    //
    // // 獲取地圖邊界
    // void GetMapBoundaries(float& left, float& right, float& top, float& bottom) const;
    //
    // // 根據相機移動更新所有磚塊位置
    // void UpdateTilePositions(float cameraDeltaX);

    const std::vector<std::shared_ptr<Character>>& GetMapTiles() const {
        return m_MapTiles;
    }

    // 獲取磚塊位置矩陣
    // const std::vector<glm::vec2>& GetTilePositions() const {
    //     return m_TilePositions;
    // }

    // float GetStartY() const { return m_StartY; }
    // float GetTileSize() const { return m_TileSize; }

private:
    // 創建地圖磚塊
    void CreateMapTiles(const std::vector<std::vector<int>>& map);

    // std::vector<std::vector<int>> m_MapData;
    std::vector<std::shared_ptr<Character>> m_MapTiles;
    //std::vector<glm::vec2> m_TilePositions;
    Util::Renderer& m_Root;

    float m_TileSize;
    float m_StartX;
    float m_StartY;
};

#endif // MAP_MANAGER_HPP