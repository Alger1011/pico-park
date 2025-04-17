#include <iostream>

#include "App.hpp"
#include "Util/Logger.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Camera.hpp"

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

void App::PassCheck() {
    if (m_pico1->IfCollides(m_key) || m_pico2->IfCollides(m_key)) {
        LOG_DEBUG("Pico collided with key!");
        m_key->SetVisible(false);
    }
    if (m_key->GetVisibility() == false && (m_pico1->IfCollides(m_door1) || m_pico2->IfCollides(m_door1))) {
        m_door1->SetImage(GA_RESOURCE_DIR"/Image/Character/door2.png");
        LOG_DEBUG("The door is open");
    }
    // pico1 進門
    if (m_pico1->IfCollides(m_door1) && Util::Input::IsKeyPressed(Util::Keycode::W)) {
        if (m_pico1->GetVisibility()) {
            m_pico1->SetVisible(false);
            LOG_DEBUG("pico1 entered the door");
        }
    }
    // pico2 進門
    if (m_pico2->IfCollides(m_door1) && Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        if (m_pico2->GetVisibility()) {
            m_pico2->SetVisible(false);
            LOG_DEBUG("pico2 entered the door");
        }
    }
}


void App::Update() {
    // ImGui::Begin("gm");
    // ImGui::DragFloat("x", &gm->m_Transform.translation.x);
    // ImGui::End();

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

    // // === 🔽 碰撞檢測 🔽 ===
    // if (m_Giraffe->IfCollides(m_Chest)) {
    //     m_Chest->SetVisible(false);  // 隱藏寶箱
    // }
    //
    // // === 🔼 碰撞檢測結束 🔼 ===

    // === 🔽 加入角色移動邏輯 🔽 ===
    // 在移動前紀錄舊位置

    const float gravity = -20.0f;     // 模擬重力
    const float groundLevel = -200.0f;  // 地面高度 (假設地面 y = 0)

    // m_pico1 (WAD 控制)
    float speed1 = 5.0f;
    glm::vec2 Position1 = m_pico1->GetPosition();
    glm::vec2 Position2 = m_pico2->GetPosition();
    float velocityY1 = 0.0f;  // 垂直速度 (用於重力與跳躍)
    m_pico1->m_Transform.translation.x -= m_Camera->Update(m_pico1, m_pico2);

    // m_pico2 (上下左右 控制)
    float speed2 = 5.0f;

    float velocityY2 = 0.0f;  // 垂直速度 (用於重力與跳躍)

    // 先用舊位置初始化新位置，稍後修改
    glm::vec2 newPosition1 = Position1;
    glm::vec2 newPosition2 = Position2;

    glm::vec2 pico1Size = m_pico1->GetScaledSize();
    glm::vec2 pico2Size = m_pico2->GetScaledSize();

    // --- feet position 判斷角色腳下方塊 ---
    glm::vec2 feetPos1 = { newPosition1.x, newPosition1.y - pico1Size.y / 2 - 0.1f };
    glm::vec2 feetPos2 = { newPosition2.x, newPosition2.y - pico2Size.y / 2 - 0.1f };

    // ---- m_pico1 移動邏輯 (WAD 控制) ----
    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        bool isOnGround = m_MapManager->HasTileAt(feetPos1);
        bool isOnOtherPico = m_pico2->GetVisibility() && m_pico2->IsStanding(m_pico1);
        bool someoneOnMyHead = m_pico2->GetVisibility() && m_pico1->IsStanding(m_pico2);
        if ((isOnGround || isOnOtherPico) && !someoneOnMyHead) {
            m_pico1->Isjumping();
        }
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        newPosition1.x -= speed1;  // 左移
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        newPosition1.x += speed1;  // 右移
    }
    // ---- m_pico2 移動邏輯 (上下左右 控制) ----
    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        bool isOnGround = m_MapManager->HasTileAt(feetPos2);
        bool isOnOtherPico = m_pico1->GetVisibility() && m_pico1->IsStanding(m_pico2);
        bool someoneOnMyHead = m_pico1->GetVisibility() && m_pico2->IsStanding(m_pico1);
        if ((isOnGround || isOnOtherPico) && !someoneOnMyHead) {
            m_pico2->Isjumping();
        }
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        newPosition2.x -= speed2;  // 左移
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        newPosition2.x += speed2;  // 右移
    }
    m_Camera->Update(m_pico1, m_pico2);

    // 相機跟隨與角色互動部分
    // // 這一整塊我先註解掉，記得把他寫成函式，不然好醜
// if (m_Phase == Phase::STAGE_FOUR) {  // 只在 Phase::STAGE_THREE 階段啟用相機跟隨和地圖顯示
//     // 如果是第一次進入此階段，初始化地圖和顯示設置
//     static bool isFirstEnterPhase3 = true;
//     if (isFirstEnterPhase3) {
//         // 顯示所有地圖磚塊
//         for (auto& tile : m_MapManager->GetMapTiles()) {
//             tile->SetVisible(true);
//         }
//
//         // 重置角色位置到適當的起始點
//         m_pico1->SetPosition({-100.0f, -140.5f});
//         m_pico2->SetPosition({50.0f, -140.5f});
//
//         isFirstEnterPhase3 = false;
//     }
//
//     // 儲存角色的世界座標
//     static glm::vec2 worldPos1(0.0f, 0.0f);
//     static glm::vec2 worldPos2(0.0f, 0.0f);
//     static bool firstRun = true;
//
//     if (firstRun) {
//         // 第一次運行時初始化世界座標
//         worldPos1 = m_pico1->GetPosition();
//         worldPos2 = m_pico2->GetPosition();
//         firstRun = false;
//     }
//
//     // 處理角色移動（使用世界座標）
//     float moveX1 = 0.0f, moveY1 = 0.0f;
//     float moveX2 = 0.0f, moveY2 = 0.0f;
//
//     // Pico1 移動控制 (WAD)
//     if (Util::Input::IsKeyDown(Util::Keycode::W)) {
//         moveY1 += 5.0f;
//     }
//     if (Util::Input::IsKeyDown(Util::Keycode::S)) {
//         moveY1 -= 5.0f;
//     }
//     if (Util::Input::IsKeyDown(Util::Keycode::A)) {
//         moveX1 -= 5.0f;
//     }
//     if (Util::Input::IsKeyDown(Util::Keycode::D)) {
//         moveX1 += 5.0f;
//     }
//
//     // Pico2 移動控制 (方向鍵)
//     if (Util::Input::IsKeyDown(Util::Keycode::UP)) {
//         moveY2 += 5.0f;
//     }
//     if (Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
//         moveY2 -= 5.0f;
//     }
//     if (Util::Input::IsKeyDown(Util::Keycode::LEFT)) {
//         moveX2 -= 5.0f;
//     }
//     if (Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
//         moveX2 += 5.0f;
//     }
//
//     // 計算新的世界座標
//     glm::vec2 newWorldPos1 = worldPos1 + glm::vec2(moveX1, moveY1);
//     glm::vec2 newWorldPos2 = worldPos2 + glm::vec2(moveX2, moveY2);
//
//     // Add this where character movement is handled in AppUpdate.cpp
//
//     // Get map boundaries for collision detection
//     float mapLeft, mapRight, mapTop, mapBottom;
//     if (m_MapManager) {
//         m_MapManager->GetMapBoundaries(mapLeft, mapRight, mapTop, mapBottom);
//     } else {
//         // Default values if map manager isn't available
//         mapLeft = -387.0f;
//         mapRight = 387.0f;
//         mapTop = 223.0f;
//         mapBottom = -223.0f;
//     }
//
//
//     // For m_pico1 movement - add after updating newPosition1
//     float width1 = 20.0f; // Approximate character width, adjust based on actual sprite
//     float height1 = 40.0f; // Approximate character height, adjust based on actual sprite
//     // Apply character boundary constraints
//     if (newPosition1.x - width1/2 < mapLeft) {
//         newPosition1.x = mapLeft + width1/2;
//     }
//     if (newPosition1.x + width1/2 > mapRight) {
//         newPosition1.x = mapRight - width1/2;
//     }
//     if (newPosition1.y + height1/2 > mapTop) {
//         newPosition1.y = mapTop - height1/2;
//     }
//     if (newPosition1.y - height1/2 < mapBottom) {
//         newPosition1.y = mapBottom + height1/2;
//     }
//
//     // For m_pico2 movement - add after updating newPosition2
//     float width2 = 25.0f; // Might be different size from pico1
//     float height2 = 50.0f; // Might be different size from pico1
//     // Apply character boundary constraints
//     if (newPosition2.x - width2/2 < mapLeft) {
//         newPosition2.x = mapLeft + width2/2;
//     }
//     if (newPosition2.x + width2/2 > mapRight) {
//         newPosition2.x = mapRight - width2/2;
//     }
//     if (newPosition2.y + height2/2 > mapTop) {
//         newPosition2.y = mapTop - height2/2;
//     }
//     if (newPosition2.y - height2/2 < mapBottom) {
//         newPosition2.y = mapBottom + height2/2;
//     }
//
//     // 確保角色不會分開太遠
//     float maxDistance = 400.0f; // 最大允許距離
//     glm::vec2 diff = newWorldPos2 - newWorldPos1;
//     float distance = glm::length(diff);
//
//     if (distance > maxDistance) {
//         // 將兩個角色都拉近到允許的最大距離
//         glm::vec2 direction = glm::normalize(diff);
//         glm::vec2 center = (newWorldPos1 + newWorldPos2) * 0.5f;
//
//         newWorldPos1 = center - direction * maxDistance * 0.5f;
//         newWorldPos2 = center + direction * maxDistance * 0.5f;
//     }
//
//     // 更新世界座標
//     worldPos1 = newWorldPos1;
//     worldPos2 = newWorldPos2;
//
//     // 更新相機
//     m_Camera->Update(m_pico1, m_pico2);
//
//     // 將世界座標轉換為螢幕座標
//     glm::vec2 screenPos1 = m_Camera->WorldToScreenPosition(worldPos1);
//     glm::vec2 screenPos2 = m_Camera->WorldToScreenPosition(worldPos2);
//
//     // 更新角色的螢幕位置
//     m_pico1->SetPosition(screenPos1);
//     m_pico2->SetPosition(screenPos2);
//
//     glm::vec2 correctedWorld1 = worldPos1;
//     if (CheckTileCollision(newPosition1, pico1Size, correctedWorld1, velocityY1)) {
//         newPosition1 = correctedWorld1;
//     }
//     glm::vec2 correctedPos2 = worldPos2;
//     if (CheckTileCollision(newPosition2, pico2Size, correctedPos2, velocityY2)) {
//         newPosition2 = correctedPos2;
//     }
//
//     // 更新地圖磚塊的位置
//     if (m_MapManager) {
//         for (auto& tile : m_MapManager->GetMapTiles()) {
//             glm::vec2 tileWorldPos = tile->GetPosition();
//             glm::vec2 tileScreenPos = m_Camera->WorldToScreenPosition(tileWorldPos);
//             tile->SetPosition(tileScreenPos);
//
//             // 檢查地圖磚塊是否在視野內，如果在則顯示，否則隱藏
//             bool isVisible = fabs(tileScreenPos.x) < m_Camera->GetViewWidth() / 2 + 50.0f &&
//                              fabs(tileScreenPos.y) < m_Camera->GetViewHeight() / 2 + 50.0f;
//             tile->SetVisible(isVisible);
//         }
//     }
// } else {
//     // 在其他階段，隱藏地圖磚塊
//     if (m_MapManager) {
//         for (auto& tile : m_MapManager->GetMapTiles()) {
//             tile->SetVisible(false);
//         }
//     }
// }

    // ---- 重力與跳躍的更新邏輯 ----
    velocityY1 += gravity * 0.2f;
    newPosition1.y += velocityY1;

    velocityY2 += gravity * 0.2f;
    newPosition2.y += velocityY2;

    // pico1 判定地磚
    if (m_MapManager->HasTileAt(feetPos1)) {
        int gridY1 = static_cast<int>((m_MapManager->GetStartY() - feetPos1.y) / m_MapManager->GetTileSize());
        float tileTopY = m_MapManager->GetStartY() - gridY1 * m_MapManager->GetTileSize();
        // LOG_DEBUG("pico1 feetY: {}, tileTopY: {}, heightOffset: {}", feetPos1.y, tileTopY, pico1Size.y / 2.0f);
        newPosition1.y = tileTopY + pico1Size.y / 2.0f;
        velocityY1 = 0.0f;
    }

    // pico2 判定地磚
    if (m_MapManager->HasTileAt(feetPos2)) {
        int gridY2 = static_cast<int>((m_MapManager->GetStartY() - feetPos2.y) / m_MapManager->GetTileSize());
        float tileTopY = m_MapManager->GetStartY() - gridY2 * m_MapManager->GetTileSize();
        // LOG_DEBUG("pico2 feetY: {}, tileTopY: {}, heightOffset: {}", feetPos2.y, tileTopY, pico2Size.y / 2.0f);
        newPosition2.y = tileTopY + pico2Size.y / 2.0f;
        velocityY2 = 0.0f;
    }

    glm::vec2 deltaPosition2 = newPosition2 - Position2;
    glm::vec2 deltaPosition1 = newPosition1 - Position1;

    // 檢查角色是否站立在對方上並調整位置與速度
    if (m_pico1->IsStanding(m_pico2)) {
        glm::vec2 pico2Pos = m_pico2->GetPosition();
        //glm::vec2 pico1Size = m_pico1->GetScaledSize();
        newPosition2.y = pico2Pos.y - pico1Size.y;
        newPosition2.y = pico2Pos.y;
        newPosition2 += deltaPosition1;
    }

    if (m_pico2->IsStanding(m_pico1)) {
        glm::vec2 pico1Pos = m_pico1->GetPosition();
        //glm::vec2 pico2Size = m_pico2->GetScaledSize();
        newPosition1.y = pico1Pos.y - pico2Size.y;
        newPosition1.y = pico1Pos.y;
        newPosition1 += deltaPosition2;
    }

    // 設定角色新位置
    m_pico1->SetPosition(newPosition1);
    m_pico2->SetPosition(newPosition2);
    // === 🔼 角色移動邏輯結束 🔼 ===

    // === 🔽 pico in stage_ons 🔽 ===
    // if (m_Phase == Phase::STAGE_ONE) {
    //     PassCheck();
    // }
    // // === 🔽 pico in stage_two 🔽 ===
    // if (m_Phase == Phase::STAGE_TWO) {
    //     PassCheck();
    // }

    PassCheck();
    m_pico1 -> Ismoving();
    m_pico2 -> Ismoving();
    m_Root.Update();


    // === 🔽 讓長頸鹿碰到門，門就打開 🔽 ===
    // if (m_Phase == Phase::STAGE_ONE) {
    //     for (const auto& door : m_Doors) {
    //         if (m_Giraffe->IfCollides(door)) {  // ✅ 碰撞檢測
    //             door->SetVisible(true);  // ✅ 門打開
    //             door->SetImage(GA_RESOURCE_DIR"/Image/Character/door_open.png");
    //         }
    //     }
    // }

    // Key 位子測試
    // ImGui::Begin("test");
    // ImGui::SetWindowSize({300, 300});
    // ImGui::DragFloat2("position", &m_key->m_Transform.translation[0],1,-100, 100);
    // ImGui::End();

}
