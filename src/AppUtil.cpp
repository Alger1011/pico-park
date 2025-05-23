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
            if (true) {
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
            if (true){
                // !m_pico1->GetVisibility() && !m_pico2->GetVisibility()
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
                auto box1 = std::make_shared<Box>(GA_RESOURCE_DIR"/Image/Character/long1.png", glm::vec2( -300, 2), glm::vec2(56,425));
                m_Objects.push_back(box1);
                m_Root.AddChild(box1);

                auto box2 = std::make_shared<Box>(GA_RESOURCE_DIR"/Image/Character/long2.png", glm::vec2( 300, 2), glm::vec2(56,425));
                m_Objects.push_back(box2);
                m_Root.AddChild(box2);

                auto key = std::make_shared<Key>(GA_RESOURCE_DIR"/Image/Character/key.png", glm::vec2(-400, 10), glm::vec2(31, 61));
                key -> SetZIndex(10);
                m_key = key;
                m_Objects.push_back(key);
                m_Root.AddChild(key);

                auto box3 = std::make_shared<Square_Box>(GA_RESOURCE_DIR"/Image/Character/square.png", glm::vec2( 1100, -155), glm::vec2(150,150));
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
            if (true){
                // !m_pico1->GetVisibility() && !m_pico2->GetVisibility()
                m_Phase = Phase::STAGE_THREE_LOADING;
                Reset();
                m_pico1->SetVisible(false);
                m_pico2->SetVisible(false);
                m_key->SetVisible(false);
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("ERROR.");
            }
        break;

        case Phase::STAGE_THREE_LOADING:
            if (true){
                m_Phase = Phase::STAGE_THREE;
                m_pico2->SetPosition({40.0f, 100.0f});
                m_pico1->SetPosition({50.0f, 0.0f});
                m_pico1->SetVisible(true);
                m_pico2->SetVisible(true);

                m_pico1 -> SetSpeed(1, -m_pico1 -> GetSpeed(1));
                m_pico2 -> SetSpeed(1, -m_pico2 -> GetSpeed(1));
                // 載入地圖
                std::string mapPath = GA_RESOURCE_DIR"/Map/third.txt";
                CreateMapTiles(mapPath);
                // 載入物件
                auto box1 = std::make_shared<Small_Box>(GA_RESOURCE_DIR"/Image/Character/brick.png", glm::vec2( 55, 250), glm::vec2(57,72));
                m_Objects.push_back(box1);
                m_Root.AddChild(box1);

                auto box2 = std::make_shared<Small_Box>(GA_RESOURCE_DIR"/Image/Character/brick.png", glm::vec2( 900, -175), glm::vec2(57,72));
                m_Objects.push_back(box2);
                m_Root.AddChild(box2);

                auto key = std::make_shared<Key>(GA_RESOURCE_DIR"/Image/Character/key.png", glm::vec2(1000, 250), glm::vec2(31, 61));
                key -> SetZIndex(10);
                m_Objects.push_back(key);
                m_Root.AddChild(key);

                auto box3 = std::make_shared<Small_Box>(GA_RESOURCE_DIR"/Image/Character/brick.png", glm::vec2( 1100, -175), glm::vec2(57,72));
                m_Objects.push_back(box3);
                m_Root.AddChild(box3);

                auto rec1 = std::make_shared<Platform1>(GA_RESOURCE_DIR"/Image/Character/rectangle 3.png", glm::vec2( 1050, -237), glm::vec2(625,55));
                m_Objects.push_back(rec1);//向左移650
                m_Root.AddChild(rec1);

                auto button1 = std::make_shared<Button>(GA_RESOURCE_DIR"/Image/Character/button.png", glm::vec2(700, 145), glm::vec2(22, 51));
                m_Objects.push_back(button1);//控制rec1及rec2
                m_Root.AddChild(button1);
                rec1 -> SetButton(button1);

                auto rec2 = std::make_shared<Platform2>(GA_RESOURCE_DIR"/Image/Character/vrec-third.png", glm::vec2( 600, 420), glm::vec2(10,16));
                m_Objects.push_back(rec2);//按到按鈕後要往下移
                m_Root.AddChild(rec2);
                rec2 -> SetButton(button1);

                auto rec3 = std::make_shared<Platform>(GA_RESOURCE_DIR"/Image/Character/vrec-third.png", glm::vec2(1500, 220), glm::vec2(200,16));
                m_Objects.push_back(rec3);//向上移
                m_Root.AddChild(rec3);

                auto button3 = std::make_shared<Button>(GA_RESOURCE_DIR"/Image/Character/button.png", glm::vec2(1300, -225), glm::vec2(22, 51));
                m_Objects.push_back(button3);//控制rec3
                m_Root.AddChild(button3);

                auto rec4 = std::make_shared<Platform>(GA_RESOURCE_DIR"/Image/Character/vrec-third.png", glm::vec2(1600, 220), glm::vec2(200,16));
                m_Objects.push_back(rec4);//向上移
                m_Root.AddChild(rec4);

                auto button4 = std::make_shared<Button>(GA_RESOURCE_DIR"/Image/Character/button.png", glm::vec2(1400, -225), glm::vec2(22, 51));
                m_Objects.push_back(button4);//控制rec4
                m_Root.AddChild(button4);

                auto rec5 = std::make_shared<Platform>(GA_RESOURCE_DIR"/Image/Character/vrec-third.png", glm::vec2(1700, 220), glm::vec2(200,16));
                m_Objects.push_back(rec5);//向上移
                m_Root.AddChild(rec5);

                auto button5 = std::make_shared<Button>(GA_RESOURCE_DIR"/Image/Character/button.png", glm::vec2(1500, -225), glm::vec2(22, 51));
                m_Objects.push_back(button5);//控制rec5
                m_Root.AddChild(button5);

                auto door = std::make_shared<Door>(GA_RESOURCE_DIR"/Image/Character/door1.png", glm::vec2(2000, -170), glm::vec2(83, 80));
                door -> SetZIndex(5);
                m_Objects.push_back(door);
                m_Root.AddChild(door);
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The door doesn't open or doesn't get the key.");
            }
        break;

        case Phase::STAGE_THREE:
            if (!m_pico1->GetVisibility() && !m_pico2->GetVisibility()){
                m_Phase = Phase::STAGE_FOUR_LOADING;
                Reset();
                m_pico1->SetVisible(false);
                m_pico2->SetVisible(false);
                m_key->SetVisible(false);
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("ERROR.");
            }
        break;

        case Phase::STAGE_FOUR_LOADING:

              if (m_pico1->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/pico_stand1.png"){
                m_Phase = Phase::STAGE_FOUR;
                m_pico2->SetPosition({50.0f, 0.0f});
                m_pico1->SetPosition({-100.0f, 0.0f});
                m_pico1->SetVisible(true);
                m_pico2->SetVisible(true);

                m_pico1 -> SetSpeed(1, -m_pico1 -> GetSpeed(1));
                m_pico2 -> SetSpeed(1, -m_pico2 -> GetSpeed(1));
                // 載入地圖
                std::string mapPath = GA_RESOURCE_DIR"/Map/forth.txt";
                CreateMapTiles(mapPath);

                // 載入物件
                auto box1 = std::make_shared<Box>(GA_RESOURCE_DIR"/Image/Character/long1.png", glm::vec2( -300, 2), glm::vec2(56,425));
                m_Objects.push_back(box1);
                m_Root.AddChild(box1);

                auto box2 = std::make_shared<Box>(GA_RESOURCE_DIR"/Image/Character/long2.png", glm::vec2( 300, 2), glm::vec2(56,425));
                m_Objects.push_back(box2);
                m_Root.AddChild(box2);

                auto key = std::make_shared<Key>(GA_RESOURCE_DIR"/Image/Character/key.png", glm::vec2(-400, 10), glm::vec2(31, 61));
                key -> SetZIndex(10);
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

        case Phase::STAGE_FOUR:
            if (!m_pico1->GetVisibility() && !m_pico2->GetVisibility()){
                m_CurrentState = State::END;
                Reset();
                m_pico1->SetVisible(false);
                m_pico2->SetVisible(false);
                m_key->SetVisible(false);
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("ERROR.");
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
    //LOG_ERROR(Total);
}

void App::Reset() {
    for (auto& obj : m_Objects) {
        m_Root.RemoveChild(obj);
    }
    m_Objects.clear();
}