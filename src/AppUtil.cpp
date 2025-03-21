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
                //m_Chest->SetVisible(true);
                Map::LoadMap("first.txt");;
                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The level is not yet available.");
            }
            break;

        case Phase::STAGE_ONE:
            if (AreAllDoorsOpen(m_Doors)) {
                if (m_Chest->GetVisibility()) {
                    LOG_DEBUG("The pico collided with the chest but the chest is still visible");
                } else {
                    m_Phase = Phase::BEE_ANIMATION;
                    m_Giraffe->SetVisible(false);
                    //m_Bee->SetVisible(true);

                    m_PRM->NextPhase();
                }
            } else {
                LOG_DEBUG("The giraffe is not colliding with the chest");
            }
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
