#include "AppUtil.hpp"
#include "App.hpp"
#include "Map.hpp"
#include "Util/Logger.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

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
                m_Phase = Phase::STAGE_ONE_LOADING;
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The image is not correct");
                // LOG_DEBUG("The image path is {} instead.", m_Giraffe->GetImagePath());
            }
        break;

        case Phase::STAGE_ONE_LOADING:
            if (m_pico1->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/right_pico_stand1.png") {
                m_Phase = Phase::STAGE_ONE;
                m_pico2->SetPosition({50.0f, 0.0f});
                m_pico1->SetPosition({-100.0f, 0.0f});
                m_pico1->SetVisible(true);
                m_pico2->SetVisible(true);
                // m_key->SetVisible(true);
                // m_door1->SetVisible(true);

                m_pico1 -> SetSpeed(1, -m_pico1 -> GetSpeed(1));
                m_pico2 -> SetSpeed(1, -m_pico2 -> GetSpeed(1));
                // 載入地圖
                std::string mapPath = GA_RESOURCE_DIR"/Map/first.txt";
                CreateMapTiles(mapPath);
                // 載入板子
                auto object = std::make_shared<Board>(GA_RESOURCE_DIR"/Image/Character/board.png", glm::vec2( 0, -100), glm::vec2(200,16));
                m_Objects.push_back(object);
                m_Root.AddChild(object);
                m_PRM->NextPhase();
            } else {
                LOG_INFO("The level is not yet available.");
            }
        break;

        case Phase::STAGE_ONE:
            if (!m_pico1->GetVisibility() && !m_pico2->GetVisibility()){
                m_door1->SetImage(GA_RESOURCE_DIR"/Image/Character/door1.png");
                m_Phase = Phase::STAGE_TWO_LOADING;
                // 角色設置不可見
                for (auto& tile : m_MapManager->GetMapTiles()) {
                    tile->SetVisible(false);
                }
                m_pico1->SetVisible(false);
                m_pico2->SetVisible(false);
                m_key->SetVisible(false);
                m_door1->SetVisible(false);
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("ERROR.");
            }
        break;

        case Phase::STAGE_TWO_LOADING:
            if (m_pico1->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/pico_stand1.png"){
                m_door1->SetImage(GA_RESOURCE_DIR"/Image/Character/door1.png");
                m_Phase = Phase::STAGE_TWO;
                // 確保地圖在 STAGE_THREE 開始時可見
                for (auto& tile : m_MapManager->GetMapTiles()) {
                    tile->SetVisible(true);
                }
                // 載入地圖
                std::string mapPath = GA_RESOURCE_DIR"/Map/second.txt";
                m_MapManager->LoadMap(mapPath);
                CreateMapTiles(m_Map);
                Map::RenderMap(m_Map);
                // 重置角色位置到適當的起始點
                m_pico1->SetPosition({-100.0f, 0.0f});
                m_pico2->SetPosition({50.0f, 0.0f});
                m_pico1->SetVisible(true);
                m_pico2->SetVisible(true);
                m_key->SetVisible(true);
                m_door1->SetVisible(true);
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The door doesn't open or doesn't get the key.");
            }
        break;

        case Phase::STAGE_TWO:
            if (!m_pico1->GetVisibility() && !m_pico2->GetVisibility()){
                m_door1->SetImage(GA_RESOURCE_DIR"/Image/Character/door1.png");
                m_Phase = Phase::OPEN_THE_DOORS;
                // 角色設置不可見
                for (auto& tile : m_MapManager->GetMapTiles()) {
                    tile->SetVisible(false);
                }
                m_pico1->SetVisible(false);
                m_pico2->SetVisible(false);
                m_key->SetVisible(false);
                m_door1->SetVisible(false);
                //_Map->SetVisible();
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("ERROR.");
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

void App::CreateMapTiles(const std::string& filename) {
    float m_TileSize = 55.0f;
    float m_StartX = -387.0f;
    float m_StartY = 223.0f;
    std::vector<std::vector<int>> matrix;
    std::ifstream file(filename);
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
    // for (size_t y = 0; y < matrix.size(); ++y) {
    //     // 計算每行最多顯示的列數
    //     size_t maxColumns = std::min(matrix[y].size(), static_cast<size_t>(16));
    //
    //     for (size_t x = 0; x < maxColumns; ++x) {
    //         if (matrix[y][x] == 1) {
    //             // 創建磚塊
    //             float posX = m_StartX + x * m_TileSize;
    //             float posY = m_StartY - y * m_TileSize;
    //             auto object = std::make_shared<Object>(GA_RESOURCE_DIR"/Image/Character/orange_tile.png", glm::vec2(posX, posY), glm::vec2(72,72));
    //
    //             m_Root.AddChild(object);
    //             m_Objects.push_back(object);
    //         }
    //     }
    // }
    int Total = 0;
    for (size_t y = 0; y < matrix.size(); ++y) {
        // 計算每行最多顯示的列數
        for (size_t x = 0; x < matrix[0].size(); ++x) {
            if (matrix[y][x] == 1) {
                // 創建磚塊
                float posX = m_StartX + x * m_TileSize;
                float posY = m_StartY - y * m_TileSize;
                auto object = std::make_shared<Object>(GA_RESOURCE_DIR"/Image/Character/orange_tile.png", glm::vec2(posX, posY), glm::vec2(72,72));
                m_Root.AddChild(object);
                m_Objects.push_back(object);
            }
            Total ++;
        }
    }
    LOG_ERROR(Total);
}
