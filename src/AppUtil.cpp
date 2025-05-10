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
            LOG_INFO("Welcome");
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

                m_pico1 -> SetSpeed(1, -m_pico1 -> GetSpeed(1));
                m_pico2 -> SetSpeed(1, -m_pico2 -> GetSpeed(1));
                // 載入地圖
                std::string mapPath = GA_RESOURCE_DIR"/Map/first.txt";
                CreateMapTiles(mapPath);
                // 載入板子
                auto object = std::make_shared<Board>(GA_RESOURCE_DIR"/Image/Character/board.png", glm::vec2( 2210, -190), glm::vec2(200,16));
                m_Objects.push_back(object);
                m_Root.AddChild(object);

                auto key = std::make_shared<Key>(GA_RESOURCE_DIR"/Image/Character/key.png", glm::vec2(2050, 140), glm::vec2(31, 61));
                key -> SetZIndex(10);
                m_key = key;
                m_Objects.push_back(key);
                m_Root.AddChild(key);

                auto door = std::make_shared<Door>(GA_RESOURCE_DIR"/Image/Character/door1.png", glm::vec2(2500, 75), glm::vec2(83, 80));
                door -> SetZIndex(5);
                m_Objects.push_back(door);
                m_Root.AddChild(door);

                auto button = std::make_shared<Button>(GA_RESOURCE_DIR"/Image/Character/button.png", glm::vec2(1500, -225), glm::vec2(22, 51));
                m_Objects.push_back(button);
                m_Root.AddChild(button);

                auto platform = std::make_shared<Platform>(GA_RESOURCE_DIR"/Image/Character/rectangle.png", glm::vec2(1540, -227), glm::vec2(300, 36));
                m_Objects.push_back(platform);
                m_Root.AddChild(platform);
                platform->SetButton(button);

                m_PRM->NextPhase();
            } else {
                LOG_INFO("The door doesn't open or doesn't get the key.");
            }
        break;

        case Phase::STAGE_ONE:
            if (!m_pico1->GetVisibility() && !m_pico2->GetVisibility()){
                m_Phase = Phase::STAGE_TWO_LOADING;
                Reset();
                m_pico1->SetVisible(false);
                m_pico2->SetVisible(false);
                m_key->SetVisible(false);
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("ERROR.");
            }
        break;

        case Phase::STAGE_TWO_LOADING:
            if (true){
                m_Phase = Phase::STAGE_TWO;
                m_pico2->SetPosition({50.0f, 0.0f});
                m_pico1->SetPosition({-100.0f, 0.0f});
                m_pico1->SetVisible(true);
                m_pico2->SetVisible(true);

                m_pico1 -> SetSpeed(1, -m_pico1 -> GetSpeed(1));
                m_pico2 -> SetSpeed(1, -m_pico2 -> GetSpeed(1));
                // 載入地圖
                std::string mapPath = GA_RESOURCE_DIR"/Map/second.txt";
                CreateMapTiles(mapPath);
                // 載入物件
                auto box1 = std::make_shared<Object>(GA_RESOURCE_DIR"/Image/Character/long1.png", glm::vec2( -300, 2), glm::vec2(200,16));
                m_Objects.push_back(box1);
                m_Root.AddChild(box1);

                auto box2 = std::make_shared<Object>(GA_RESOURCE_DIR"/Image/Character/long2.png", glm::vec2( 300, 2), glm::vec2(200,16));
                m_Objects.push_back(box2);
                m_Root.AddChild(box2);

                auto key = std::make_shared<Key>(GA_RESOURCE_DIR"/Image/Character/key.png", glm::vec2(-400, 10), glm::vec2(31, 61));
                key -> SetZIndex(10);
                m_key = key;
                m_Objects.push_back(key);
                m_Root.AddChild(key);

                auto box3 = std::make_shared<Object>(GA_RESOURCE_DIR"/Image/Character/square.png", glm::vec2( 1100, -155), glm::vec2(200,16));
                m_Objects.push_back(box3);
                m_Root.AddChild(box3);

                auto door = std::make_shared<Door>(GA_RESOURCE_DIR"/Image/Character/door1.png", glm::vec2(1800, -108), glm::vec2(83, 80));
                door -> SetZIndex(5);
                m_Objects.push_back(door);
                m_Root.AddChild(door);

                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The door doesn't open or doesn't get the key.");
            }
        break;

        case Phase::STAGE_TWO:
            if (!m_pico1->GetVisibility() && !m_pico2->GetVisibility()){
                m_Phase = Phase::OPEN_THE_DOORS;
                Reset();
                m_pico1->SetVisible(false);
                m_pico2->SetVisible(false);
                m_key->SetVisible(false);
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

    // 磚塊設定---------------
    void App::CreateMapTiles(const std::string& filename) {
        float m_TileSize = 62.0f;
        float m_StartX = -500.0f;
        float m_StartY = 251.0f;
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

void App::Reset() {
    for (auto& obj : m_Objects) {
        m_Root.RemoveChild(obj);
    }
    m_Objects.clear();
}