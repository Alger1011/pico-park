#include "App.hpp"
#include "Camera.hpp"
#include "./Util/GameObject.hpp"
#include "Util/Logger.hpp"
#include "MapManager.hpp"


void App::Start() {
    LOG_TRACE("Start");
    // gm = std::make_shared<Util::GameObject>();
    //
    //
    // m_Root.AddChild(gm);
  
    m_Camera = std::make_unique<Camera>(800.0f, 600.0f);
    IsCleared = false;

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

    m_key = std::make_shared<Key>(GA_RESOURCE_DIR"/Image/Character/key.png", glm::vec2(2050, 110), glm::vec2(31, 61));
    m_key->SetZIndex(5);
    m_Root.AddChild(m_key);

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

    m_PRM = std::make_shared<PhaseResourceManger>();
    m_Root.AddChildren(m_PRM->GetChildren());

    m_CurrentState = State::UPDATE;
}