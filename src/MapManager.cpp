#include "MapManager.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "Util/Logger.hpp"

MapManager::MapManager(Util::Renderer& root)
    : m_Root(root)
    , m_TileSize(55.0f)
    , m_StartX(-387.0f)
    , m_StartY(223.0f)
{
}

void MapManager::LoadMap(const std::string& filename) {
    // 清除現有的地圖磚塊
    m_MapTiles.clear();

    // 載入地圖數據
    m_MapData = Map::LoadMap(filename);

    // 檢查地圖是否為空
    if (m_MapData.empty()) {
        LOG_ERROR("Map is empty, cannot create tiles");
        return;
    }

    // 創建地圖磚塊
    CreateMapTiles(m_MapData);
}

void MapManager::CreateMapTiles(const std::vector<std::vector<int>>& map) {
    LOG_DEBUG("Creating Map Tiles - Total Map Rows: {}", map.size());

    std::string tileImagePath = GA_RESOURCE_DIR"/Image/Character/orange_tile.png";
    int tilesCreated = 0;

    for (size_t y = 0; y < map.size(); ++y) {
        // 計算每行最多顯示的列數
        size_t maxColumns = std::min(map[y].size(), static_cast<size_t>(16));

        for (size_t x = 0; x < maxColumns; ++x) {
            if (map[y][x] == 1) {
                // 創建磚塊
                auto tile = std::make_shared<Character>(tileImagePath);
                float posX = m_StartX + x * m_TileSize;
                float posY = m_StartY - y * m_TileSize;

                tile->SetPosition({posX, posY});
                tile->SetZIndex(5); // 確保磚塊在背景之上，但在角色之下
                tile->SetVisible(true);

                m_Root.AddChild(tile);
                m_MapTiles.push_back(tile);
                tilesCreated++;
            }
        }
    }

    LOG_INFO("Created {} map tiles", tilesCreated);
}

// In MapManager.cpp - add or update the UpdateVisibility method
void MapManager::UpdateVisibility(const Camera& camera) {
    // Get camera view dimensions
    float viewWidth = camera.GetViewWidth();
    float viewHeight = camera.GetViewHeight();

    // Get camera position
    glm::vec2 cameraPos = camera.GetPosition();

    // Update each tile's visibility
    for (const auto& tile : m_MapTiles) {
        glm::vec2 tilePos = tile->GetPosition();

        // Calculate tile's screen position relative to camera
        glm::vec2 screenPos = camera.WorldToScreenPosition(tilePos);

        // Check if tile is within view (with some margin)
        float margin = m_TileSize;
        bool isVisible = (std::abs(screenPos.x) < viewWidth * 0.5f + margin &&
                          std::abs(screenPos.y) < viewHeight * 0.5f + margin);

        // Update tile position and visibility
        tile->SetPosition(screenPos);
        tile->SetVisible(isVisible);
    }
}

bool MapManager::HasTileAt(const glm::vec2& position) const {

    int gridX = static_cast<int>((position.x - m_StartX) / m_TileSize);
    int gridY = static_cast<int>((m_StartY - position.y) / m_TileSize);

    if (gridY >= 0 && gridY < m_MapData.size() &&
        gridX >= 0 && gridX < m_MapData[gridY].size()) {
        return m_MapData[gridY][gridX] == 1;
    }

    return false;
}

void MapManager::GetMapBoundaries(float& left, float& right, float& top, float& bottom) const {
    if (m_MapData.empty()) {
        left = right = top = bottom = 0.0f;
        return;
    }

    size_t maxWidth = 0;
    for (const auto& row : m_MapData) {
        maxWidth = std::max(maxWidth, row.size());
    }

    left = m_StartX;
    right = m_StartX + maxWidth * m_TileSize;
    top = m_StartY;
    bottom = m_StartY - m_MapData.size() * m_TileSize;
}
