#include "Camera.hpp"
#include "Character.hpp"
#include "Util/Logger.hpp"
#include "App.hpp"
#include "MapManager.hpp"  // 添加 MapManager 頭文件
#include <algorithm>
#include <cmath>
#include <cfloat>

Camera::Camera(float viewWidth, float viewHeight)
    : m_Position(0.0f, 0.0f)
    , m_ViewWidth(viewWidth)
    , m_ViewHeight(viewHeight)
    , m_LeftBoundary(-FLT_MAX)
    , m_RightBoundary(FLT_MAX)
    , m_TopBoundary(FLT_MAX)
    , m_BottomBoundary(-FLT_MAX)
    // , m_SmoothFactor(0.1f)
    // , dif_post(0.0f)
    , m_MapManager(nullptr)
{
    LOG_INFO("Camera initialized with view dimensions: {}x{}", viewWidth, viewHeight);
}

// void Camera::ForceCharactersInView(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2) {
//     glm::vec2 pico1Pos = pico1->GetPosition();
//     glm::vec2 pico2Pos = pico2->GetPosition();
//
//     // Calculate screen positions
//     glm::vec2 screenPos1 = WorldToScreenPosition(pico1Pos);
//     glm::vec2 screenPos2 = WorldToScreenPosition(pico2Pos);
//
//     // Define screen boundaries with some margin
//     float margin = 50.0f;
//     float leftBound =  m_Position.x - 450.0f ;
//     float rightBound = m_Position.x + 450.0f;
//     float topBound = m_ViewHeight * 0.5f - margin;
//     float bottomBound = -m_ViewHeight * 0.5f + margin;
//
//     // Check if character 1 is going off-screen and correct
//     bool pico1OutOfBounds = (screenPos1.x < leftBound || screenPos1.x > rightBound ||
//                              screenPos1.y < bottomBound || screenPos1.y > topBound);
//
//     // Check if character 2 is going off-screen and correct
//     bool pico2OutOfBounds = (screenPos2.x < leftBound || screenPos2.x > rightBound ||
//                              screenPos2.y < bottomBound || screenPos2.y > topBound);
//
//     // Calculate max distance between characters based on screen size
//     float maxDistance = glm::min(m_ViewWidth, m_ViewHeight) * 0.8f;
//     glm::vec2 diff = pico2Pos - pico1Pos;
//     float distance = glm::length(diff);
//
//     // If characters are too far apart or one is off-screen, pull them together
//     if (distance > maxDistance || pico1OutOfBounds || pico2OutOfBounds) {
//         if (distance > 0.0f) {
//             glm::vec2 direction = glm::normalize(diff);
//
//             // If characters are too far apart, move them closer
//             if (distance > maxDistance) {
//                 float adjustment = (distance - maxDistance) * 0.5f;
//                 pico1->SetPosition(pico1Pos + direction * adjustment);
//                 pico2->SetPosition(pico2Pos - direction * adjustment);
//             }
//
//             // If one character is off-screen, pull them both toward center
//             if (pico1OutOfBounds) {
//                 pico1->SetPosition(pico2Pos - direction * maxDistance * 0.8f);
//             }
//
//             if (pico2OutOfBounds) {
//                 pico2->SetPosition(pico1Pos + direction * maxDistance * 0.8f);
//             }
//         }
//     }
// }

// float Camera::Update(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2) {
//     // 保存舊的相機位置以計算位移量
//     float old_pos = m_Position.x;
//
//     // 計算兩個角色的世界座標
//     static glm::vec2 worldPos1(0.0f, 0.0f);
//     static glm::vec2 worldPos2(0.0f, 0.0f);
//     worldPos1 = pico1->GetPosition();
//     worldPos2 = pico2->GetPosition();
//
//     // 更新相機位置為兩個角色的中點
//     m_Position.x = (worldPos1.x + worldPos2.x) / 2;
//
//     // 計算相機 X 軸的位移量
//     dif_post = m_Position.x - old_pos;
//
//     // 檢查是否有角色超出視圖並處理
//     ForceCharactersInView(pico1, pico2);
//
//     // 更新地圖磚塊的位置（如果 MapManager 已設置）
//     if (m_MapManager != nullptr && dif_post != 0.0f) {
//         m_MapManager->UpdateTilePositions(dif_post);
//         LOG_DEBUG("Camera moved by dX: {}, updating map tiles", dif_post);
//     }
//
//     return dif_post;
// }
//確定不能刪
void Camera::SetBoundaries(float left, float right, float top, float bottom) {
    m_LeftBoundary = left;
    m_RightBoundary = right;
    m_TopBoundary = top;
    m_BottomBoundary = bottom;

    LOG_INFO("Camera boundaries set to: L={}, R={}, T={}, B={}", left, right, top, bottom);
}

// glm::vec2 Camera::WorldToScreenPosition(const glm::vec2& worldPos) const {
//     // 世界座標轉換為螢幕座標
//     return worldPos - m_Position;
// }
//
// glm::vec2 Camera::ScreenToWorldPosition(const glm::vec2& screenPos) const {
//     // 螢幕座標轉換為世界座標
//     return screenPos + m_Position;
// }

// glm::vec2 Camera::CheckBoundaries(const glm::vec2& characterPos, const glm::vec2& characterSize) const {
//     glm::vec2 newPos = characterPos;
//
//     // 檢查並修正角色位置，確保不會超出地圖邊界
//     float halfWidth = characterSize.x * 0.5f;
//     float halfHeight = characterSize.y * 0.5f;
//
//     newPos.x = std::clamp(newPos.x, m_LeftBoundary + halfWidth, m_RightBoundary - halfWidth);
//     newPos.y = std::clamp(newPos.y, m_BottomBoundary + halfHeight, m_TopBoundary - halfHeight);
//
//     return newPos;
// }

// bool Camera::IsCharacterOutOfBounds(const std::shared_ptr<Character>& character) const {
//     glm::vec2 position = character->GetPosition();
//
//     // Calculate the visible screen boundaries
//     float halfWidth = m_ViewWidth * 0.5f;
//     float halfHeight = m_ViewHeight * 0.5f;
//
//     // Convert character position to screen space relative to camera
//     glm::vec2 screenPos = WorldToScreenPosition(position);
//
//     // Check if character is outside the visible area
//     bool outOfBounds = (screenPos.x < -halfWidth || screenPos.x > halfWidth ||
//                          screenPos.y < -halfHeight || screenPos.y > halfHeight);
//
//     return outOfBounds;
// }