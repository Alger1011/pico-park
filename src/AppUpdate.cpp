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
    if (m_door1 -> GetVisibility() && m_pico1->IfCollides(m_door1) && Util::Input::IsKeyPressed(Util::Keycode::W) && m_door1->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/door2.png") {
        if (m_pico1->GetVisibility()) {
            m_pico1->SetVisible(false);
            LOG_DEBUG("pico1 entered the door");
        }
    }
    // pico2 進門
    if (m_door1 -> GetVisibility() && m_pico2->IfCollides(m_door1) && Util::Input::IsKeyPressed(Util::Keycode::UP) && m_door1->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/door2.png") {
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

    const float jump = 30.0f;
    const float gravity = 1.0f;     // 模擬重力
    const float speed = 5.0f;

    // m_pico1 (WAD 控制)
    // glm::vec2 Position1 = m_pico1->GetPosition();
    // glm::vec2 Position2 = m_pico2->GetPosition();
    // glm::vec2 Board_Position = m_board1->GetPosition();
    // m_pico1->m_Transform.translation.x -= m_Camera->Update(m_pico1, m_pico2);
    // float board_speed = 5.0f;

    m_pico1 -> SetSpeed(1, gravity);
    m_pico2 -> SetSpeed(1, gravity);

    // 先用舊位置初始化新位置，稍後修改
    // glm::vec2 newPosition1 = Position1;
    // glm::vec2 newPosition2 = Position2;
    // glm::vec2 new_Board_Position = Board_Position;

    // glm::vec2 pico1Size = m_pico1->GetScaledSize();
    // glm::vec2 pico2Size = m_pico2->GetScaledSize();

    // --- feet position 判斷角色腳下方塊 ---
    // glm::vec2 feetPos1 = { newPosition1.x, newPosition1.y - pico1Size.y / 2 - 0.1f };
    // glm::vec2 feetPos2 = { newPosition2.x, newPosition2.y - pico2Size.y / 2 - 0.1f };

    bool someoneOnMyHead = m_pico2->GetVisibility() && m_pico1->IsStanding(m_pico2);
    if (someoneOnMyHead) {
        m_pico1 -> SetJumpState(false);
        m_pico2 -> SetOnHead(true);
        m_pico2 -> SetSpeed(1, -m_pico2 -> GetSpeed(1));
        m_pico2 -> SetJumpState(true);
    }
    else {
        m_pico2 -> SetOnHead(false);
        someoneOnMyHead = m_pico1->GetVisibility() && m_pico2->IsStanding(m_pico1);
        if (someoneOnMyHead) {
            m_pico2 -> SetJumpState(false);
            m_pico1 -> SetOnHead(true);
            m_pico1 -> SetSpeed(1, -m_pico1 -> GetSpeed(1));
            m_pico1 -> SetJumpState(true);
        }
        else {
            m_pico1 -> SetOnHead(false);
        }
    }
    // LOG_ERROR(m_pico1 -> GetJumpState());
    // LOG_ERROR(m_pico2 -> GetJumpState());

    // ---- m_pico1 移動邏輯 (WAD 控制) ----
    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        // bool isOnGround = m_MapManager->HasTileAt(feetPos1);
        // bool isOnOtherPico = m_pico2->GetVisibility() && m_pico2->IsStanding(m_pico1);
        // bool someoneOnMyHead = m_pico2->GetVisibility() && m_pico1->IsStanding(m_pico2);
        // if ((isOnGround || isOnOtherPico) && !someoneOnMyHead) {
        //     m_pico1 -> SetSpeed(0, jump);
        // }
        if (m_pico1 -> GetJumpState()) {
            if (m_pico1 -> GetSpeed(0) == 0) {
                m_pico1 -> SetSpeed(0, jump);
            }
            m_pico1 -> SetJumpState(false);
        }
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        m_pico1 -> SetSpeed(2, speed);  // 左移
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        m_pico1 -> SetSpeed(3, speed);  // 右移
    }
    // ---- m_pico2 移動邏輯 (上下左右 控制) ----
    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        // bool isOnGround = m_MapManager->HasTileAt(feetPos2);
        // bool isOnOtherPico = m_pico1->GetVisibility() && m_pico1->IsStanding(m_pico2);
        // bool someoneOnMyHead = m_pico1->GetVisibility() && m_pico2->IsStanding(m_pico1);
        // if ((isOnGround || isOnOtherPico) && !someoneOnMyHead) {
        //     m_pico2->SetSpeed(0, jump);
        // }
        if (m_pico2 -> GetJumpState()) {
            if (m_pico2 -> GetSpeed(0) == 0) {
                m_pico2 -> SetSpeed(0, jump);
            }
            m_pico2 -> SetJumpState(false);
        }
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        m_pico2 -> SetSpeed(2, speed);  // 左移
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        m_pico2 -> SetSpeed(3, speed);  // 右移
    }
    // m_Camera->Update(m_pico1, m_pico2);

    // pico1 判定地磚
    // if (m_MapManager->HasTileAt(feetPos1)) {
    //     int gridY1 = static_cast<int>((m_MapManager->GetStartY() - feetPos1.y) / m_MapManager->GetTileSize());
    //     float tileTopY = m_MapManager->GetStartY() - gridY1 * m_MapManager->GetTileSize();
    //     // LOG_DEBUG("pico1 feetY: {}, tileTopY: {}, heightOffset: {}", feetPos1.y, tileTopY, pico1Size.y / 2.0f);
    //     newPosition1.y = tileTopY + pico1Size.y / 2.0f;
    //     // velocityY1 = 0.0f;
    // }

    // pico2 判定地磚
    // if (m_MapManager->HasTileAt(feetPos2)) {
    //     int gridY2 = static_cast<int>((m_MapManager->GetStartY() - feetPos2.y) / m_MapManager->GetTileSize());
    //     float tileTopY = m_MapManager->GetStartY() - gridY2 * m_MapManager->GetTileSize();
    //     // LOG_DEBUG("pico2 feetY: {}, tileTopY: {}, heightOffset: {}", feetPos2.y, tileTopY, pico2Size.y / 2.0f);
    //     newPosition2.y = tileTopY + pico2Size.y / 2.0f;
    //     // velocityY2 = 0.0f;
    // }

    // glm::vec2 deltaPosition2 = newPosition2 - Position2;
    // glm::vec2 deltaPosition1 = newPosition1 - Position1;
    // glm::vec2 delta_Board_Position = new_Board_Position - Board_Position;

    // 檢查角色是否站立在對方上並調整位置與速度
    // if (m_pico1->IsStanding(m_pico2)) {
    //     glm::vec2 pico2Pos = m_pico2->GetPosition();
    //     newPosition2.y = pico2Pos.y - pico1Size.y;
    //     newPosition2.y = pico2Pos.y;
    //     newPosition2 += deltaPosition1;
    // }
    // if (m_pico2->IsStanding(m_pico1)) {
    //     glm::vec2 pico1Pos = m_pico1->GetPosition();
    //     //glm::vec2 pico2Size = m_pico2->GetScaledSize();
    //     newPosition1.y = pico1Pos.y - pico2Size.y;
    //     newPosition1.y = pico1Pos.y;
    //     newPosition1 += deltaPosition2;
    // }

    // 站在板子上
    // float gound = -150.0f;
    // if (m_pico1->IsStandingOnBoard(m_board1) || m_pico2->IsStandingOnBoard(m_board1)) {
    //     new_Board_Position += board_speed;
    //     glm::vec2 pico1Pos = m_pico1->GetPosition();
    //     glm::vec2 BoardSize = m_board1->GetScaledSize();
    //     newPosition1.y = pico1Pos.y - BoardSize.y;
    //     newPosition1.y = pico1Pos.y;
    //     newPosition1 += delta_Board_Position;
    // }
    // else if (new_Board_Position.y >= gound) {
    //     new_Board_Position -= board_speed;
    // }

    // 設定角色新位置
    // m_pico1->SetPosition(newPosition1);
    // m_pico2->SetPosition(newPosition2);
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
    bool SomeOneOnHead = m_pico1 -> GetOnHead() || m_pico2 -> GetOnHead();
    for (auto& obj : m_Objects) {
        for (auto& pico : m_pico) {
            int result = pico -> IfCollidesObject(obj);
            if (result == 0) {
                pico -> SetSpeed(1, -pico -> GetSpeed(1));
                pico -> PositionCorrection(0, obj);
                pico -> SetJumpState(true);
                if (obj -> GetType() == "Board") {
                    if (SomeOneOnHead) {
                        obj -> AddCurrNumber(m_pico1);
                        obj -> AddCurrNumber(m_pico2);
                    }
                    else {
                        obj -> AddCurrNumber(pico);
                    }
                }
            }
            if (result == 1) {
                pico -> SetSpeed(0, -pico -> GetSpeed(0));
                pico -> PositionCorrection(1, obj);
            }
            if (result == 2) {
                pico -> SetSpeed(3, -pico -> GetSpeed(3));
                pico -> PositionCorrection(2, obj);
            }
            if (result == 3) {
                pico -> SetSpeed(2, -pico -> GetSpeed(2));
                pico -> PositionCorrection(3, obj);
            }
        }
    }

    if (m_pico1 -> GetOnHead()) {
        m_pico1 -> HeadCorrection(m_pico2);
        m_pico1 -> SetSpeed(0, m_pico2 -> GetSpeed(0));
        m_pico1 -> SetSpeed(1, m_pico2 -> GetSpeed(1));
        m_pico1 -> SetSpeed(2, m_pico2 -> GetSpeed(2));
        m_pico1 -> SetSpeed(3, m_pico2 -> GetSpeed(3));
    }

    if (m_pico2 -> GetOnHead()) {
        m_pico2 -> HeadCorrection(m_pico1);
        m_pico2 -> SetSpeed(0, m_pico1 -> GetSpeed(0));
        m_pico2 -> SetSpeed(1, m_pico1 -> GetSpeed(1));
        m_pico2 -> SetSpeed(2, m_pico1 -> GetSpeed(2));
        m_pico2 -> SetSpeed(3, m_pico1 -> GetSpeed(3));
    }

    float centerX = (m_pico1 -> m_Transform.translation.x + m_pico2 -> m_Transform.translation.x) / 2;
    float diff = 0;
    for (auto& pico : m_pico) {
        if (pico -> m_Transform.translation.x > 430) {
            diff += pico -> GetSpeed(3);
        }
        else if (pico -> m_Transform.translation.x < -430) {
            diff -= pico -> GetSpeed(2);
        }
    }
    if (diff == 0) {
        diff = centerX - 0;
    }
    for (auto& obj : m_Objects) {
        obj ->m_Transform.translation.x -= diff;
    }
    m_pico1 -> m_Transform.translation.x -= diff;
    m_pico2 -> m_Transform.translation.x -= diff;



    for (auto& pico : m_pico) {
        if (pico -> m_Transform.translation.x < -500) {
            pico -> m_Transform.translation.x = -500 + pico -> GetSize().x/2;
        }
        else if (pico -> m_Transform.translation.x > 500) {
            pico -> m_Transform.translation.x = 500 - pico -> GetSize().x/2;
        }
    }
    // glm::vec2 position = Util::Input::GetCursorPosition ();
    // LOG_ERROR(position);

    m_pico1 -> Ismoving();
    m_pico2 -> Ismoving();

    for (auto obj : m_Objects) {
        obj -> Move();
        obj -> ResetCurrNumber();
    }

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
