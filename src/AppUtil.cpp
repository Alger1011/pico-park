#include "AppUtil.hpp"
#include "App.hpp"
#include "Map.hpp"
#include "Util/Logger.hpp"

/**
 * @brief The function to validate the tasks.
 * @warning Do not modify this function.
 * @note See README.md for the task details.
 */
void App::ValidTask() {
    bool isBeeLooping;
    bool isBeePlaying;
    LOG_DEBUG("Validating the task {}", static_cast<int>(m_Phase));
    switch (m_Phase) {
        case Phase::Welcome:
            if (m_Giraffe->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/giraffe.png") {
                m_Phase = Phase::PICK_STAGE;
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The image is not correct");
                // LOG_DEBUG("The image path is {} instead.", m_Giraffe->GetImagePath());
            }
        break;

        case Phase::PICK_STAGE:
            if (m_pico1->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/pico_stand1.png") {
                m_Phase = Phase::STAGE_ONE;
                m_pico2->SetPosition({50.0f, -140.5f});
                m_pico1->SetPosition({-100.0f, -155.5f});
                m_pico1->SetVisible(true);
                m_pico2->SetVisible(true);
                // 載入地圖
                std::string mapPath = GA_RESOURCE_DIR"/Map/first.txt";
                m_Map = Map::LoadMap(mapPath);

                // 創建視覺化地圖磚塊
                CreateMapTiles(m_Map);
                // 在控制台渲染地圖以便調試
                Map::RenderMap(m_Map);

                m_PRM->NextPhase();
            } else {
                LOG_INFO("The level is not yet available.");
            }
        break;

        case Phase::STAGE_ONE:
            m_Phase = Phase::BEE_ANIMATION;
        m_Giraffe->SetVisible(false);
        m_Bee->SetVisible(true);
        m_PRM->NextPhase();
        break;

        case Phase::BEE_ANIMATION:
            isBeeLooping = m_Bee->IsLooping();
        isBeePlaying = m_Bee->IsPlaying();

        if (isBeeLooping && isBeePlaying) {
            m_Phase = Phase::OPEN_THE_DOORS;
            m_Giraffe->SetPosition({-112.5f, -140.5f});
            m_Giraffe->SetVisible(true);
            m_Bee->SetVisible(false);
            std::for_each(m_Doors.begin(), m_Doors.end(), [](const auto& door) { door->SetVisible(true); });

            m_PRM->NextPhase();
        } else {
            LOG_DEBUG("The bee animation is {} but not {}", isBeeLooping ? "looping" : "playing",
                      isBeeLooping ? "playing" : "looping");
        }
        break;

        case Phase::OPEN_THE_DOORS:
            if (AreAllDoorsOpen(m_Doors)) {
                m_Phase = Phase::COUNTDOWN;
                std::for_each(m_Doors.begin(), m_Doors.end(), [](const auto& door) { door->SetVisible(false); });
                m_Giraffe->SetVisible(false);

                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("At least one door is not open");
            }
        break;

        case Phase::COUNTDOWN:
            if (m_Ball->IfAnimationEnds()) {
                LOG_DEBUG("Congratulations! You have completed Giraffe Adventure!");
                m_CurrentState = State::END;
            } else{
                LOG_DEBUG("The ball animation is not ended");
            }
        break;
    }
}

// 在 AppUtil.cpp 中添加
void App::CreateMapTiles(const std::vector<std::vector<int>>& map){
    // 先清除現有的地圖磚塊
    m_MapTiles.clear();

    // 檢查地圖是否為空
    if (map.empty()){
        LOG_ERROR("Map is empty, cannot create tiles");
        return;
    }

    LOG_DEBUG("Creating Map Tiles - Total Map Rows: {}", map.size());

    float tileSize = 32.0f;
    float startX = -450.0f; // 調整起始位置，使地圖更居中
    float startY = 100.0f;  // 調整起始位置，使地圖更靠上

    int tilesCreated = 0;

    std::string tileImagePath = GA_RESOURCE_DIR"/Image/Character/orange_tile.png";
    for (size_t y = 0; y < map.size(); ++y) {
        for (size_t x = 0; x < map[y].size(); ++x) {
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
std::vector<std::shared_ptr<Character>> m_MapTiles;