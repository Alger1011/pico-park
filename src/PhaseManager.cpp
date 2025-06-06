#include "PhaseResourceManger.hpp"

#include "Util/Logger.hpp"

PhaseResourceManger::PhaseResourceManger() {
    m_TaskText = std::make_shared<TaskText>();
    m_Background = std::make_shared<BackgroundImage>();
}

void PhaseResourceManger::NextPhase() {
    if (m_Phase == 9) return;
    // LOG_INFO("Passed! Next phase: {}", m_Phase);
    if (!m_Background) {
        // LOG_ERROR("m_Background is null!");
    } else {
        // LOG_INFO("Calling m_Background->NextPhase");
        m_Background->NextPhase(m_Phase);  // 可能 crash
        // LOG_INFO("Finished m_Background->NextPhase");
    }
    if (!m_TaskText) {
        // LOG_ERROR("m_TaskText is null!");
    } else {
        // LOG_INFO("Calling m_TaskText->NextPhase");
        m_TaskText->NextPhase(m_Phase++);  // 可能 crash
        // LOG_INFO("Finished m_TaskText->NextPhase");
    }
}
