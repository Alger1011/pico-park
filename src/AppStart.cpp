#include "App.hpp"
#include "Camera.hpp"
#include "./Util/GameObject.hpp"
#include "Util/Logger.hpp"
#include "Map.hpp"
#include "MapManager.hpp"


void App::Start() {
    LOG_TRACE("Start");
    // gm = std::make_shared<Util::GameObject>();
    //
    //
    // m_Root.AddChild(gm);
  
    m_Camera = std::make_unique<Camera>(800.0f, 600.0f);


    // 玩家 1（PICO1）
    m_pico1 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/right_pico_stand1.png");
    m_pico1->m_Transform.translation = {-100.0f, -155.5f};
    m_pico1->SetZIndex(50);
    m_pico1->SetVisible(false);
    m_pico1->m_Transform.scale = glm::vec2(0.198f, 0.17f);  // 放大 0.25 倍
    m_Root.AddChild(m_pico1);

    // 玩家 2（PICO2）
    m_pico2 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/right_pico_stand2.png");
    m_pico2->m_Transform.translation = {50.0f, -155.5f};
    m_pico2->SetZIndex(50);
    m_pico2->SetVisible(false);
    m_pico2->m_Transform.scale = glm::vec2(0.195f, 0.17f);  // 放大 0.25 倍
    m_Root.AddChild(m_pico2);

    m_pico.push_back(m_pico1);
    m_pico.push_back(m_pico2);

    m_Giraffe = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/giraffe.png");
    m_Giraffe->SetPosition({-112.5f, -140.5f});
    m_Giraffe->SetVisible(false);
    m_Giraffe->SetZIndex(50);
    m_Root.AddChild(m_Giraffe);

    m_Chest = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/chest.png");
    m_Chest->SetZIndex(5);
    m_Chest->SetPosition({197.5f, -3.5f});
    m_Chest->SetVisible(false);
    m_Root.AddChild(m_Chest);

    m_key = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/key.png");
    m_key->SetZIndex(5);
    m_key->SetPosition({50.5f, -10.5f});
    m_key->SetVisible(false);
    m_key->m_Transform.scale = glm::vec2(0.5f, 0.5f);
    m_Root.AddChild(m_key);

    m_door1 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door1.png");
    m_door1->SetZIndex(5);
    m_door1->SetPosition({150.5f, -150.5f});
    m_door1->m_Transform.scale = glm::vec2(0.5f, 0.5f);
    m_door1->SetVisible(false);
    m_Root.AddChild(m_door1);

    m_board1 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/board2.png");
    m_board1->SetZIndex(5);
    m_board1->SetPosition({320.5f, -150.5f});
    //m_board1->m_Transform.scale = glm::vec2(0.5f, 0.5f);
    m_board1->SetVisible(false);
    m_Root.AddChild(m_board1);

    m_MapManager = std::make_unique<MapManager>(m_Root);

    std::string mapPath = GA_RESOURCE_DIR"/Map/first.txt";
    m_MapManager->LoadMap(mapPath);

    m_Camera->SetMapManager(m_MapManager.get());

    // 設置相機邊界
    float left = -500.0f;
    float right = 500.0f;
    float top = 223.0f;
    float bottom = -223.0f;
    m_Camera->SetBoundaries(left, right, top, bottom);

    for (auto& tile : m_MapManager->GetMapTiles()) {
        tile->SetVisible(false);
    }

    std::vector<std::string> beeImages;
    beeImages.reserve(2);
    for (int i = 0; i < 2; ++i) {
        beeImages.emplace_back(GA_RESOURCE_DIR"/Image/Character/bee_" + std::to_string(i + 1) + ".png");
    }

    m_Bee = std::make_shared<AnimatedCharacter>(beeImages);
    m_Bee->SetZIndex(5);
    m_Bee->SetVisible(false);
    m_Root.AddChild(m_Bee);

    for (int i = 0; i < 3; ++i) {
        m_Doors.push_back(std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png"));
        m_Doors[i]->SetZIndex(5);
        m_Doors[i]->SetPosition({205.5f - 80.f * i, -3.5f});
        m_Doors[i]->SetVisible(false);
        m_Root.AddChild(m_Doors[i]);
    }

    // TODO: The counting down ball for phase 6

    m_PRM = std::make_shared<PhaseResourceManger>();
    m_Root.AddChildren(m_PRM->GetChildren());

    m_CurrentState = State::UPDATE;
}