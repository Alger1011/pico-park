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
                m_key->SetVisible(true);
                m_door1->SetVisible(true);
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
            //LOG_DEBUG("Key Pos: ({}, {})", m_key->GetPosition().x, m_key->GetPosition().y);

            if (m_door1->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/door2.png"){
                m_door1->SetImage(GA_RESOURCE_DIR"/Image/Character/door1.png");
                m_Phase = Phase::BEE_ANIMATION;
                // 確保地圖在 STAGE_THREE 開始時可見
                for (auto& tile : m_MapManager->GetMapTiles()) {
                    tile->SetVisible(true);
                }
                // 重置角色位置到適當的起始點
                m_pico1->SetPosition({-100.0f, -155.5f});
                m_pico2->SetPosition({50.0f, -155.5f});
                m_pico1->SetVisible(true);
                m_pico2->SetVisible(true);

                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The door doesn't open or doesn't get the key.");
            }
        break;

        case Phase::BEE_ANIMATION:
            isBeeLooping = m_Bee->IsLooping();
            isBeePlaying = m_Bee->IsPlaying();

            if (isBeeLooping && isBeePlaying) {
                m_Phase = Phase::STAGE_THREE; // 改為直接進入 STAGE_THREE
                m_Giraffe->SetVisible(false);
                m_Bee->SetVisible(false);
                // 確保地圖在 STAGE_THREE 開始時可見
                for (auto& tile : m_MapManager->GetMapTiles()) {
                tile->SetVisible(true);
                }
                // 重置角色位置到適當的起始點
                m_pico1->SetPosition({-100.0f, -155.5f});
                m_pico2->SetPosition({50.0f, -155.5f});
                m_pico1->SetVisible(true);
                m_pico2->SetVisible(true);

                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The bee animation is {} but not {}", isBeeLooping ? "looping" : "playing",
                      isBeeLooping ? "playing" : "looping");
            }
        break;

        case Phase::STAGE_THREE:
            // 完成第三階段後的處理
            m_Phase = Phase::OPEN_THE_DOORS;
            m_Giraffe->SetPosition({-112.5f, -140.5f});
            m_Giraffe->SetVisible(true);
            m_Bee->SetVisible(false);
            // 隱藏地圖磚塊
            for (auto& tile : m_MapManager->GetMapTiles()) {
                tile->SetVisible(false);
            }
            std::for_each(m_Doors.begin(), m_Doors.end(), [](const auto& door) { door->SetVisible(true); });

            m_PRM->NextPhase();
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
