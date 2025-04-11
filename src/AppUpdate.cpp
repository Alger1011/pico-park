#include <iostream>

#include "App.hpp"
#include "Util/Logger.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

bool App::CheckTileCollision(glm::vec2 charPos, glm::vec2 charSize, glm::vec2& correctedPos, float& velocityY){
    for (auto& tile : m_MapTiles) {
        glm::vec2 tilePos = tile->GetPosition();
        glm::vec2 tileSize = {55.0f, 55.0f}; // 你的 tile 大小

        bool overlapX = abs(charPos.x - tilePos.x) < (charSize.x + tileSize.x) / 2;
        bool overlapY = abs(charPos.y - tilePos.y) < (charSize.y + tileSize.y) / 2;

        if (overlapX && overlapY) {
            if (charPos.y > tilePos.y) {
                correctedPos.y = tilePos.y + tileSize.y / 2 + charSize.y / 2;
                velocityY = 0.0f;
                return true;
            }
        }
    }
    return false;
}


void App::Update() {

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // 簡化 Enter 按鍵處理
    //if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
    //    ValidTask();
    //}

    if (m_EnterDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
            ValidTask();
       }
    }
    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::RETURN);

    // === 🔽 碰撞檢測 🔽 ===
    if (m_Giraffe->IfCollides(m_Chest)) {
        m_Chest->SetVisible(false);  // 隱藏寶箱
    }

    // === 🔼 碰撞檢測結束 🔼 ===

    // === 🔽 加入角色移動邏輯 🔽 ===
    // 在移動前紀錄舊位置

    const float gravity = -20.0f;     // 模擬重力
    const float groundLevel = -140.0f;  // 地面高度 (假設地面 y = 0)

    // m_pico1 (WAD 控制)
    float speed1 = 5.0f;
    glm::vec2 Position1 = m_pico1->GetPosition();
    float velocityY1 = 0.0f;  // 垂直速度 (用於重力與跳躍)

    // m_pico2 (上下左右 控制)
    float speed2 = 5.0f;
    glm::vec2 Position2 = m_pico2->GetPosition();
    float velocityY2 = 0.0f;  // 垂直速度 (用於重力與跳躍)

    // 先用舊位置初始化新位置，稍後修改
    glm::vec2 newPosition1 = Position1;
    glm::vec2 newPosition2 = Position2;

    // ---- m_pico1 移動邏輯 (WAD 控制) ----
    if (Util::Input::IsKeyPressed(Util::Keycode::W) && (newPosition1.y <= groundLevel || m_pico2->IsStanding(m_pico1))) {
        m_pico1 -> Isjumping();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        newPosition1.x -= speed1;  // 左移
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        newPosition1.x += speed1;  // 右移
    }
    // ---- m_pico2 移動邏輯 (上下左右 控制) ----
    if (Util::Input::IsKeyPressed(Util::Keycode::UP) && (newPosition2.y <= groundLevel || m_pico1->IsStanding(m_pico2))){
        m_pico2 -> Isjumping();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        newPosition2.x -= speed2;  // 左移
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        newPosition2.x += speed2;  // 右移
    }

    m_Camera->Update(m_pico1, m_pico2);

    // 相機跟隨與角色互動部分
// In AppUpdate.cpp - replace or modify the existing camera follow code
if (m_Phase == Phase::STAGE_THREE) {
    // Make sure map tiles are visible in this phase
    static bool firstTimeInPhase3 = true;
    if (firstTimeInPhase3) {
        for (auto& tile : m_MapManager->GetMapTiles()) {
            tile->SetVisible(true);
        }
        m_pico1->SetVisible(true);
        m_pico2->SetVisible(true);
        firstTimeInPhase3 = false;
    }

    // Update character positions based on input
    glm::vec2 newPosition1 = m_pico1->GetPosition();
    glm::vec2 newPosition2 = m_pico2->GetPosition();

    // Handle character 1 movement
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        newPosition1.x -= 5.0f;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        newPosition1.x += 5.0f;
    }

    // Handle character 2 movement
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        newPosition2.x -= 5.0f;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        newPosition2.x += 5.0f;
    }

    // Apply character movement with boundary checks
    float mapLeft, mapRight, mapTop, mapBottom;
    m_MapManager->GetMapBoundaries(mapLeft, mapRight, mapTop, mapBottom);

    // Apply boundary constraints for character 1
    float width1 = 25.0f;  // Approximate width
    float height1 = 50.0f; // Approximate height
    newPosition1.x = glm::clamp(newPosition1.x, mapLeft + width1 * 0.5f, mapRight - width1 * 0.5f);
    newPosition1.y = glm::clamp(newPosition1.y, mapBottom + height1 * 0.5f, mapTop - height1 * 0.5f);

    // Apply boundary constraints for character 2
    float width2 = 25.0f;
    float height2 = 50.0f;
    newPosition2.x = glm::clamp(newPosition2.x, mapLeft + width2 * 0.5f, mapRight - width2 * 0.5f);
    newPosition2.y = glm::clamp(newPosition2.y, mapBottom + height2 * 0.5f, mapTop - height2 * 0.5f);

    // Update character positions
    m_pico1->SetPosition(newPosition1);
    m_pico2->SetPosition(newPosition2);

    // For m_pico1 movement - add after updating newPosition1
    float width1 = 20.0f; // Approximate character width, adjust based on actual sprite
    float height1 = 40.0f; // Approximate character height, adjust based on actual sprite
    // Apply character boundary constraints
    if (newPosition1.x - width1/2 < mapLeft) {
        newPosition1.x = mapLeft + width1/2;
    }
    if (newPosition1.x + width1/2 > mapRight) {
        newPosition1.x = mapRight - width1/2;
    }
    if (newPosition1.y + height1/2 > mapTop) {
        newPosition1.y = mapTop - height1/2;
    }
    if (newPosition1.y - height1/2 < mapBottom) {
        newPosition1.y = mapBottom + height1/2;
    }

    // Update camera to follow characters
    m_Camera->Update(m_pico1, m_pico2);

    // Update map tiles based on camera position
    for (auto& tile : m_MapManager->GetMapTiles()) {
        glm::vec2 worldPos = tile->GetPosition();
        glm::vec2 screenPos = m_Camera->WorldToScreenPosition(worldPos);
        tile->SetPosition(screenPos);

        // Only show tiles that are within the camera view (with some margin)
        float margin = 100.0f;
        bool isVisible = (std::abs(screenPos.x) < m_Camera->GetViewWidth() * 0.5f + margin &&
                          std::abs(screenPos.y) < m_Camera->GetViewHeight() * 0.5f + margin);
        tile->SetVisible(isVisible);
    }
}

    // In AppUpdate.cpp - Add this in the Update method where character movement is handled

// Check if characters are out of bounds and output a signal
bool pico1OutOfBounds = m_Camera->IsCharacterOutOfBounds(m_pico1);
bool pico2OutOfBounds = m_Camera->IsCharacterOutOfBounds(m_pico2);

if (pico1OutOfBounds) {
    LOG_DEBUG("Pico1 is out of bounds! Position: ({}, {})",
                m_pico1->GetPosition().x, m_pico1->GetPosition().y);
}

if (pico2OutOfBounds) {
    LOG_DEBUG("Pico2 is out of bounds! Position: ({}, {})",
                m_pico2->GetPosition().x, m_pico2->GetPosition().y);
}

// If either character is out of bounds, set a visual indicator or play a sound
if (pico1OutOfBounds || pico2OutOfBounds) {
    // You could add a visual indicator here, like changing character color
    // Or set a flag to be used elsewhere in your code
    m_IsCharacterOutOfBounds = true;

    // You could also force characters to stay within bounds
    if (pico1OutOfBounds) {
        // Adjust pico1's position to keep in bounds
        glm::vec2 pos = m_pico1->GetPosition();
        glm::vec2 screenPos = m_Camera->WorldToScreenPosition(pos);

        // Calculate the adjustment needed
        float halfWidth = m_Camera->GetViewWidth() * 0.5f;
        float halfHeight = m_Camera->GetViewHeight() * 0.5f;

        if (screenPos.x < -halfWidth) screenPos.x = -halfWidth;
        if (screenPos.x > halfWidth) screenPos.x = halfWidth;
        if (screenPos.y < -halfHeight) screenPos.y = -halfHeight;
        if (screenPos.y > halfHeight) screenPos.y = halfHeight;

        // Convert back to world position and set
        m_pico1->SetPosition(m_Camera->ScreenToWorldPosition(screenPos));
      glm::vec2 pico1Size = m_pico1->GetScaledSize();
      glm::vec2 pico2Size = m_pico2->GetScaledSize();

      glm::vec2 correctedWorld1 = worldPos1;
      if (CheckTileCollision(newPosition1, pico1Size, correctedWorld1, velocityY1)) {
          newPosition1 = correctedWorld1;
      }
      glm::vec2 correctedPos2 = worldPos2;
      if (CheckTileCollision(newPosition2, pico2Size, correctedPos2, velocityY2)) {
          newPosition2 = correctedPos2;
    }

    // 更新地圖磚塊的位置
    if (m_MapManager) {
        for (auto& tile : m_MapManager->GetMapTiles()) {
            glm::vec2 tileWorldPos = tile->GetPosition();
            glm::vec2 tileScreenPos = m_Camera->WorldToScreenPosition(tileWorldPos);
            tile->SetPosition(tileScreenPos);

            // 檢查地圖磚塊是否在視野內，如果在則顯示，否則隱藏
            bool isVisible = fabs(tileScreenPos.x) < m_Camera->GetViewWidth() / 2 + 50.0f &&
                             fabs(tileScreenPos.y) < m_Camera->GetViewHeight() / 2 + 50.0f;
            tile->SetVisible(isVisible);
        }
    }

    if (pico2OutOfBounds) {
        // Similar adjustment for pico2
        glm::vec2 pos = m_pico2->GetPosition();
        glm::vec2 screenPos = m_Camera->WorldToScreenPosition(pos);

        float halfWidth = m_Camera->GetViewWidth() * 0.5f;
        float halfHeight = m_Camera->GetViewHeight() * 0.5f;

        if (screenPos.x < -halfWidth) screenPos.x = -halfWidth;
        if (screenPos.x > halfWidth) screenPos.x = halfWidth;
        if (screenPos.y < -halfHeight) screenPos.y = -halfHeight;
        if (screenPos.y > halfHeight) screenPos.y = halfHeight;

        m_pico2->SetPosition(m_Camera->ScreenToWorldPosition(screenPos));
    }
} else {
    m_IsCharacterOutOfBounds = false;
}

    // ---- 重力與跳躍的更新邏輯 ----

    velocityY1 += gravity * 0.2f;
    newPosition1.y += velocityY1;

    velocityY2 += gravity * 0.2f;
    newPosition2.y += velocityY2;

    // 防止角色穿透地面
    if (newPosition1.y < groundLevel) {
        newPosition1.y = groundLevel;
        velocityY1 = 0.0f;
    }
    if (newPosition2.y < groundLevel) {
        newPosition2.y = groundLevel;
        velocityY2 = 0.0f;
    }

    glm::vec2 deltaPosition2 = newPosition2 - Position2;
    glm::vec2 deltaPosition1 = newPosition1 - Position1;

    // 檢查角色是否站立在對方上並調整位置與速度
    if (m_pico1->IsStanding(m_pico2)) {
        glm::vec2 pico2Pos = m_pico2->GetPosition();
        glm::vec2 pico1Size = m_pico1->GetScaledSize();
        newPosition2.y = pico2Pos.y - pico1Size.y;
        newPosition2.y = pico2Pos.y;
        newPosition2 += deltaPosition1;
    }

    if (m_pico2->IsStanding(m_pico1)) {
        glm::vec2 pico1Pos = m_pico1->GetPosition();
        glm::vec2 pico2Size = m_pico2->GetScaledSize();
        newPosition1.y = pico1Pos.y - pico2Size.y;
        newPosition1.y = pico1Pos.y;
        newPosition1 += deltaPosition2;
    }

    // 設定角色新位置
    m_pico1->SetPosition(newPosition1);
    m_pico2->SetPosition(newPosition2);


    // === 🔼 角色移動邏輯結束 🔼 ===

    // === 🔽 讓長頸鹿碰到門，門就打開 🔽 ===
    // if (m_Phase == Phase::STAGE_ONE) {
    //     for (const auto& door : m_Doors) {
    //         if (m_Giraffe->IfCollides(door)) {  // ✅ 碰撞檢測
    //             door->SetVisible(true);  // ✅ 門打開
    //             door->SetImage(GA_RESOURCE_DIR"/Image/Character/door_open.png");
    //         }
    //     }
    // }

    // === 🔽 pico in stage_ons 🔽 ===
    if (m_Phase == Phase::STAGE_ONE) {
        if (m_pico1->IfCollides(m_key) || m_pico2->IfCollides(m_key)) {
            LOG_DEBUG("Pico collided with key!");
            m_key->SetVisible(false);
        }
        if (m_key->GetVisibility() == false && (m_pico1->IfCollides(m_door1) || m_pico2->IfCollides(m_door1))) {
            m_door1->SetImage(GA_RESOURCE_DIR"/Image/Character/door2.png");
            LOG_DEBUG("The door is open");
        }
    }

    m_pico1 -> Ismoving();
    m_pico2 -> Ismoving();
    m_Root.Update();
}
