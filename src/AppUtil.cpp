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
                m_Phase = Phase::STAGE_ONE_LOADING;
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The image is not correct");
                // LOG_DEBUG("The image path is {} instead.", m_Giraffe->GetImagePath());
            }
        break;

        case Phase::STAGE_ONE_LOADING:
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
                //m_door1->SetImage(GA_RESOURCE_DIR"/Image/Character/door1.png");
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
                m_pico1->SetPosition({-100.0f, -155.5f});
                m_pico2->SetPosition({50.0f, -155.5f});
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
