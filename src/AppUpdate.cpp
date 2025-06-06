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


void App::Update() {
    // ImGui::Begin("gm");
    // ImGui::DragFloat("x", &gm->m_Transform.translation.x);
    // ImGui::End();

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::Q)) {
        ValidTask();
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

    // === 🔽 加入角色移動邏輯 🔽 ===
    const float jump = 25.0f;
    const float gravity = 1.0f;
    const float speed = 6.0f;

    m_pico1 -> SetSpeed(1, gravity);
    m_pico2 -> SetSpeed(1, gravity);

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

    // ---- m_pico 移動邏輯 (WAD 控制) --------
    if (Util::Input::IsKeyPressed(Util::Keycode::W) && !IsCleared) {
        if (m_pico1 -> GetJumpState()) {
            if (m_pico1 -> GetSpeed(0) == 0) {
                m_pico1 -> SetSpeed(0, jump);
            }
            m_pico1 -> SetJumpState(false);
        }
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A) && !IsCleared) {
        m_pico1 -> SetSpeed(2, speed);  // 左移
        m_pico1->SetImage(GA_RESOURCE_DIR"/Image/Character/left_pico_stand1.png");
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D) && !IsCleared) {
        m_pico1 -> SetSpeed(3, speed);  // 右移
        m_pico1->SetImage(GA_RESOURCE_DIR"/Image/Character/right_pico_stand1.png");
    }
    // ---- m_pico2 移動邏輯 (上下左右 控制) ----
    if (Util::Input::IsKeyPressed(Util::Keycode::UP) && !IsCleared) {
        if (m_pico2 -> GetJumpState()) {
            if (m_pico2 -> GetSpeed(0) == 0) {
                m_pico2 -> SetSpeed(0, jump);
            }
            m_pico2 -> SetJumpState(false);
        }
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT) && !IsCleared) {
        m_pico2 -> SetSpeed(2, speed);  // 左移
        m_pico2->SetImage(GA_RESOURCE_DIR"/Image/Character/left_pico_stand2.png");
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && !IsCleared) {
        m_pico2 -> SetSpeed(3, speed);  // 右移
        m_pico2->SetImage(GA_RESOURCE_DIR"/Image/Character/right_pico_stand2.png");
    }
    // ---- m_pico 移動邏輯 (WAD 控制) ----------

    // PassCheck();

    // 物體與pico的碰撞----------------
    bool SomeOneOnHead = m_pico1 -> GetOnHead() || m_pico2 -> GetOnHead();
    for (auto& obj : m_Objects) {
        for (auto& pico : m_pico) {
            if (obj->GetType() == "Key" && obj->IsBound()) continue;
            int result = pico -> IfCollidesObject(obj);
            if (result == 0) {
                obj -> AddCollision(0);
                pico -> SetSpeed(1, -pico -> GetSpeed(1));
                pico -> PositionCorrection(0, obj);
                pico -> SetJumpState(true);
                if (obj -> GetType() == "Board" || obj -> GetType() == "Button" || obj -> GetType() == "Board1") {
                    if (SomeOneOnHead) {
                        obj -> AddCurrNumber(m_pico1);
                        obj -> AddCurrNumber(m_pico2);
                    }
                    else {
                        obj -> AddCurrNumber(pico);
                    }
                }
                if (obj -> GetType() == "Key" && !obj -> IsBound()) {
                    obj->BindTo(pico);
                }
                if (obj -> GetType() == "Button1") {
                    obj -> AddCurrNumber(pico);
                    pico -> SetPosition({ pico -> GetPosition().x, pico -> GetPosition().y - 5.0f});
                }
                // if (obj -> GetType() == "Small_Box") {
                //     if (obj -> StandOn() != nullptr) {
                //         pico -> SetOnHead(true);
                //     }
                // }
            }
            else if (result == 1) {
                obj -> AddCollision(1);
                pico -> SetSpeed(0, -pico -> GetSpeed(0));
                pico -> PositionCorrection(1, obj);
                if (obj -> GetType() == "Key" && !obj -> IsBound()) {
                    obj -> BindTo(pico);
                }
                if (obj -> GetType() == "Small_Box") {
                    pico -> SetJumpState(false);
                    obj -> SetFall(false);
                    obj -> StandOnCharacter(pico);
                    obj -> GetPicoSpeed( pico -> GetSpeed(2), pico -> GetSpeed(3));
                }
            }
            else if (result == 2) {
                // if (obj -> GetType() != "Small_Box") {
                //     obj -> AddCollision(2);
                //     pico -> SetSpeed(3, -pico -> GetSpeed(3));
                //     pico -> PositionCorrection(2, obj);
                // }
                // else {
                //     pico -> SetSpeed(3, -pico -> GetSpeed(3));
                //     pico -> PositionCorrection(2, obj);
                // }
                obj -> AddCollision(2);
                pico -> SetSpeed(3, -pico -> GetSpeed(3));
                pico -> PositionCorrection(2, obj);
                if (obj -> GetType() == "Key" && !obj -> IsBound()) {
                    obj -> BindTo(pico);
                }
                if (obj -> GetType() == "Box" || obj -> GetType() == "Square_Box" || obj -> GetType() == "Small_Box") {
                    bool p1Pushing = pico == m_pico1 && Util::Input::IsKeyPressed(Util::Keycode::D);
                    bool p2Pushing = pico == m_pico2 && Util::Input::IsKeyPressed(Util::Keycode::RIGHT);
                    if (p1Pushing || p2Pushing) {
                        obj -> Push(4.0f);
                        if (pico -> GetSide() == 2) {
                            if ((pico == m_pico2 && Util::Input::IsKeyPressed(Util::Keycode::D)) || (pico == m_pico1 && Util::Input::IsKeyPressed(Util::Keycode::RIGHT))) {
                                obj -> AddCurrNumber(m_pico1);
                                obj -> AddCurrNumber(m_pico2);
                            }
                        }
                        else {
                            obj -> AddCurrNumber(pico);
                        }
                    }
                }
                // if (obj -> GetType() == "Square_Box") {
                //     bool p1Pushing = pico == m_pico1 && Util::Input::IsKeyPressed(Util::Keycode::D);
                //     bool p2Pushing = pico == m_pico2 && Util::Input::IsKeyPressed(Util::Keycode::RIGHT);
                //     if (p1Pushing || p2Pushing) {
                //         obj -> Push(4.0f);
                //         obj -> AddCurrNumber(pico);
                //     }
                // }
            }
            else if (result == 3) {
                obj -> AddCollision(3);
                pico -> SetSpeed(2, -pico -> GetSpeed(2));
                pico -> PositionCorrection(3, obj);
                if (obj -> GetType() == "Key" && !obj -> IsBound()) {
                    obj->BindTo(pico);
                }
                if (obj -> GetType() == "Box" || obj -> GetType() == "Square_Box" || obj -> GetType() == "Small_Box") {
                    bool p1Pushing = pico == m_pico1 && Util::Input::IsKeyPressed(Util::Keycode::A);
                    bool p2Pushing = pico == m_pico2 && Util::Input::IsKeyPressed(Util::Keycode::LEFT);
                    if (p1Pushing || p2Pushing) {
                            obj -> Push(-4.0f);
                        if (pico -> GetSide() == 1) {
                            if ((pico == m_pico2 && Util::Input::IsKeyPressed(Util::Keycode::A)) || (pico == m_pico1 && Util::Input::IsKeyPressed(Util::Keycode::LEFT))) {
                                obj -> AddCurrNumber(m_pico1);
                                obj -> AddCurrNumber(m_pico2);
                            }
                        }
                        else {
                            obj -> AddCurrNumber(pico);
                        }
                    }
                }
                // if (obj -> GetType() == "Square_Box") {
                //     bool p1Pushing = pico == m_pico1 && Util::Input::IsKeyPressed(Util::Keycode::A);
                //     bool p2Pushing = pico == m_pico2 && Util::Input::IsKeyPressed(Util::Keycode::LEFT);
                //     if (p1Pushing || p2Pushing) {
                //         obj -> Push(-4.0f);
                //         obj -> AddCurrNumber(pico);
                //     }
                // }
            }
            else if (result == 4) {
                if (m_key -> GetBoundPico() == pico && obj -> GetImagePath() == GA_RESOURCE_DIR"/Image/Character/door1.png") {
                    obj -> SetImage(GA_RESOURCE_DIR"/Image/Character/door2.png");
                    m_key -> SetVisible(false);
                }
                if (pico == m_pico1 && Util::Input::IsKeyPressed(Util::Keycode::W) && obj -> GetImagePath() == GA_RESOURCE_DIR"/Image/Character/door2.png") {
                    pico -> SetVisible(false);
                    // LOG_INFO("pico1 entered the door");
                }
                else if (pico == m_pico2 && Util::Input::IsKeyPressed(Util::Keycode::UP) && obj -> GetImagePath() == GA_RESOURCE_DIR"/Image/Character/door2.png") {
                    pico -> SetVisible(false);
                    // LOG_INFO("pico1 entered the door");
                }
            }
        }
    }

    // 物體與pico的碰撞-----------
    // pico之間的碰撞------------
    int Cha1_result = m_pico1 -> IfCollidesCha(m_pico2);
    if (Cha1_result == 0) {
        m_pico1 -> SetSpeed(1, -m_pico1 -> GetSpeed(1));
        m_pico1 -> ChaPositionCorrection(0, m_pico2);
        m_pico1 -> SetJumpState(true);
        m_pico1 -> SetSide(0);
    }
    else if (Cha1_result == 1) {
        m_pico1 -> SetSpeed(0, -m_pico1 -> GetSpeed(0));
        m_pico1 -> ChaPositionCorrection(1, m_pico2);
        m_pico1 -> SetSide(0);
    }
    else if (Cha1_result == 2) {
        m_pico1 -> SetSpeed(3, -m_pico1 -> GetSpeed(3));
        m_pico1 -> ChaPositionCorrection(2, m_pico2);
        m_pico1 -> SetSide(1);
    }
    else if (Cha1_result == 3) {
        m_pico1 -> SetSpeed(2, -m_pico1 -> GetSpeed(2));
        m_pico1 -> ChaPositionCorrection(3, m_pico2);
        m_pico1 -> SetSide(2);
    }
    else {
        m_pico1 -> SetSide(0);
    }
    if (Cha1_result == -1) {
        m_key -> SetHoldKey(true);
    }
    else if (m_key -> IsBound()){
        m_key -> BindTo(m_pico1);
    }

    int Cha2_result = m_pico2 -> IfCollidesCha(m_pico1);
    if (Cha2_result == 0) {
        m_pico2 -> SetSpeed(1, -m_pico2 -> GetSpeed(1));
        m_pico2 -> ChaPositionCorrection(0, m_pico1);
        m_pico2 -> SetJumpState(true);
        m_pico2 -> SetSide(0);
    }
    else if (Cha2_result == 1) {
        m_pico2 -> SetSpeed(0, -m_pico2 -> GetSpeed(0));
        m_pico2 -> ChaPositionCorrection(1, m_pico1);
        m_pico2 -> SetSide(0);
    }
    else if (Cha2_result == 2) {
        m_pico2 -> SetSpeed(3, -m_pico2 -> GetSpeed(3));
        m_pico2 -> ChaPositionCorrection(2, m_pico1);
        m_pico2 -> SetSide(1);
    }
    else if (Cha2_result == 3) {
        m_pico2 -> SetSpeed(2, -m_pico2 -> GetSpeed(2));
        m_pico2 -> ChaPositionCorrection(3, m_pico1);
        m_pico2 -> SetSide(2);
    }
    else {
        m_pico2 -> SetSide(0);
    }
    if (Cha2_result == -1) {
        m_key -> SetHoldKey(true);
    }
    else if (m_key -> IsBound()){
        m_key -> BindTo(m_pico2);
    }
    //物體與物體的碰撞------------------------
    for (auto& obj1 : m_Objects) {
        for (auto& obj2 : m_Objects) {
            //if (obj1 == obj2) continue;
            if (obj1 -> GetType() == "Box") {
                // LOG_INFO(111);
                int result = obj1 -> IfCollidesObj(obj2);
                if (result == 0) {
                    obj1 -> AddCollision(1);
                    // obj1 -> SetSpeed(1, -obj1 -> GetSpeed(1));
                    obj1 -> ObjPositionCorrection(0, obj2);
                    obj1 -> SetFall(false);
                }
                if (result == 1) {
                    obj1 -> AddCollision(0);
                    // obj1 -> SetSpeed(0, -obj1 -> GetSpeed(0));
                    obj1 -> ObjPositionCorrection(1, obj2);
                }
                if (result == 2) {
                    obj1 -> AddCollision(3);
                    // LOG_INFO("2Box collided with {}", obj2->GetType());
                    obj1 -> ObjPositionCorrection(2, obj2);
                }
                if (result == 3) {
                    // LOG_INFO("3Box collided with {}", obj2->GetType());
                    obj1 -> AddCollision(2);
                    obj1 -> ObjPositionCorrection(3, obj2);
                }
            }
            if ( obj1 -> GetType() == "Square_Box" ){
                int result = obj1 -> IfCollidesObj(obj2);
                if (result == 0) {
                    obj1 -> AddCollision(1);
                    obj1 -> ObjPositionCorrection(0, obj2);
                }
                if (result == 1) {
                    obj1 -> AddCollision(0);
                    obj1 -> ObjPositionCorrection(1, obj2);
                }
                if (result == 2) {
                    obj1 -> AddCollision(3);
                    obj1 -> ObjPositionCorrection(2, obj2);
                }
                if (result == 3) {
                    obj1 -> AddCollision(2);
                    obj1 -> ObjPositionCorrection(3, obj2);
                }
            }
            if ( obj1 -> GetType() == "Small_Box" ) {
                int result = obj1 -> IfCollidesObj(obj2);
                if (result == 0) {
                    if (obj2 -> GetType() == "Button1") {
                        obj2 -> AddCurrPico();
                        // obj1 -> SetPosition({ obj1 -> GetPosition().x, obj1 -> GetPosition().y - 5.0f});
                    }
                    else {
                        obj1 -> AddCollision(1);
                        obj1 -> ObjPositionCorrection(0, obj2);
                        obj1 -> SetFall(false);
                    }
                }
                else if (result == 1) {
                    obj1 -> AddCollision(0);
                    // obj1 -> SetSpeed(0, -obj1 -> GetSpeed(0));
                    obj1 -> ObjPositionCorrection(1, obj2);
                }
                else if (result == 2) {
                    if (obj2 -> GetType() != "Small_Box" && obj2 -> GetType() != "Button1") {
                        obj1 -> AddCollision(3);
                        obj1 -> ObjPositionCorrection(2, obj2);
                    }
                    if (!obj1 -> GetCurrNumber().empty()) {
                        if (obj2 -> GetType() == "Small_Box" && obj1 -> GetCurrNumber().size() + obj1 -> GetCurrPico() >= 1 && obj2 -> GetCollision(3) == 0) {
                            obj2 -> AddCurrPico();
                            obj2 -> Push(4);
                            for (auto& next : m_Objects) {
                                if (next == obj1) continue;
                                int result1 = obj2 -> IfCollidesObj(next);
                                if (result1 == 2 && next -> GetType() == "Small_Box" && next -> GetCollision(3) == 0) {
                                    next -> AddCurrPico();
                                    next -> Push(4);
                                }
                                else if (next -> GetCollision(3) != 0) {
                                    obj1 -> AddCollision(3);
                                    obj2 -> AddCollision(3);
                                }
                            }
                        }
                        else if (obj2 -> GetType() != "Small_Box" && obj2 -> GetType() != "Button1") {
                            obj2 -> Push(0);
                        }
                        if (obj2 -> GetType() == "Button1") {
                            obj2 -> AddCurrPico();
                            obj1 -> SetPosition({ obj1 -> GetPosition().x, obj1 -> GetPosition().y - 5.0f});
                        }
                    }
                }
                else if (result == 3) {
                    if (obj2 -> GetType() != "Small_Box" && obj2 -> GetType() != "Button1") {
                        obj1 -> AddCollision(2);
                        obj1 -> ObjPositionCorrection(3, obj2);
                    }
                    if (!obj1 -> GetCurrNumber().empty()) {
                        if (obj2 -> GetType() == "Small_Box" && obj1 -> GetCurrNumber().size() + obj1 -> GetCurrPico() >= 1 && obj2 -> GetCollision(2) == 0) {
                            obj2 -> AddCurrPico();
                            obj2 -> Push(-4);
                            for (auto& next : m_Objects) {
                                if (next == obj1) continue;
                                int result1 = obj2 -> IfCollidesObj(next);
                                if (result1 == 3 && next -> GetType() == "Small_Box" &&  next -> GetCollision(2) == 0) {
                                    next -> AddCurrPico();
                                    next -> Push(-4);
                                }
                                else if (next -> GetCollision(2) != 0) {
                                    obj1 -> AddCollision(2);
                                    obj2 -> AddCollision(2);
                                }
                            }
                        }
                        else if (obj2->GetType() != "Small_Box"  && obj2 -> GetType() != "Button1") {
                            obj2 -> Push(0);
                        }
                    }
                    if (obj2 -> GetType() == "Button1") {
                        obj2 -> AddCurrPico();
                        obj1 -> SetPosition({ obj1 -> GetPosition().x, obj1 -> GetPosition().y - 5.0f});
                    }
                }
            }
        }
    }

    //------------------------
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
    //------------------------
    // 新的相机邏辑
    float centerX = (m_pico1->m_Transform.translation.x + m_pico2->m_Transform.translation.x) / 2;
    float diff = 0;

    // 檢查是否接近右邊
    bool nearRightEdge = false;
    for (auto& pico : m_pico) {
        if (pico->m_Transform.translation.x > 460 && pico->GetSpeed(3) > 0) {
            nearRightEdge = true;
            diff += pico->GetSpeed(3);
        }
    }

    // 檢查是否接近左邊
    bool nearLeftEdge = false;
    for (auto& pico : m_pico) {
        if (pico->m_Transform.translation.x < -460 && pico->GetSpeed(2) > 0) {
            nearLeftEdge = true;
            diff -= pico->GetSpeed(2);
        }
    }

    // 檢查最左侧的磚块是否已经出现
    bool leftmostTileVisible = false;
    float leftmostTileX = 9999.0f;
    for (auto& obj : m_Objects) {
        if (obj->m_Transform.translation.x < leftmostTileX) {
            leftmostTileX = obj->m_Transform.translation.x;
        }
    }

    // 如果最左側已经出現在屏幕，標記可見
    if (leftmostTileX > -465) {
        leftmostTileVisible = true;
    }

    bool rightmostTileVisible = false;
    float rightmostTileX = -9999.0f;
    for (auto& obj : m_Objects) {
        if (obj->m_Transform.translation.x > rightmostTileX) {
            rightmostTileX = obj->m_Transform.translation.x;
        }
    }

    // 如果最右側已经出現在屏幕，標記可見
    if (rightmostTileX < 465) {
        rightmostTileVisible = true;
    }

    // 情況1: 靠近邊緣且該邊緣的磚塊尚未全部可見時，移動相機
    if ((nearRightEdge && !rightmostTileVisible) || (nearLeftEdge && !leftmostTileVisible)) {
        // 應用相機移動
        for (auto& obj : m_Objects) {
            obj->m_Transform.translation.x -= diff;
        }
        m_pico1->m_Transform.translation.x -= diff;
        m_pico2->m_Transform.translation.x -= diff;
    }
    // 情況2: 角色中心點遠離中心，且邊緣尚未全部可見時，調整相機
    else if (!leftmostTileVisible && !rightmostTileVisible) {
        diff = centerX - 0;  // 根據角色中心點調整
        if (diff != 0) {  // 確保只在需要調整時才進行調整
            for (auto& obj : m_Objects) {
                obj->m_Transform.translation.x -= diff;
            }
            m_pico1->m_Transform.translation.x -= diff;
            m_pico2->m_Transform.translation.x -= diff;
        }
    }
    // 情況3: 當左邊界可見但右邊界不可見，且角色向右移動超過480時，進行糾正
    else if (leftmostTileVisible && !rightmostTileVisible && centerX > 0) {
        float correction = centerX - 0;
        for (auto& obj : m_Objects) {
            obj->m_Transform.translation.x -= correction;
        }
        m_pico1->m_Transform.translation.x -= correction;
        m_pico2->m_Transform.translation.x -= correction;
    }
    // 情況4: 當右邊界可見但左邊界不可見，且角色向左移動超過-480時，進行糾正
    else if (!leftmostTileVisible && rightmostTileVisible && centerX < 0) {
        float correction = centerX + 0;
        for (auto& obj : m_Objects) {
            obj->m_Transform.translation.x -= correction;
        }
        m_pico1->m_Transform.translation.x -= correction;
        m_pico2->m_Transform.translation.x -= correction;
    }

    for (auto& pico : m_pico) {
        bool z = false;
        if (pico -> m_Transform.translation.x < -500) {
            pico -> m_Transform.translation.x = -500 + pico -> GetSize().x/2;
            z = true;
        }
        else if (pico -> m_Transform.translation.x > 500) {
            pico -> m_Transform.translation.x = 500 - pico -> GetSize().x/2;
            z = true;
        }
        if (z){
            for (auto& obj : m_Objects) {
                int result = pico -> IfCollidesObject(obj);
                if (result == 2) {
                    float x = pico -> m_Transform.translation.x + 100.0f;
                    pico -> SetPosition({x, 100.0f});
                    pico -> SetSpeed(1, -pico -> GetSpeed(1));
                }
                else if (result == 3) {
                    float x = pico -> m_Transform.translation.x - 100.0f;
                    pico -> SetPosition({x, 100.0f});
                    pico -> SetSpeed(1, -pico -> GetSpeed(1));
                }
            }
        }
    }
    // 通關動畫-------------
    if (!m_pico1->GetVisibility() && !m_pico2->GetVisibility() && IsStage) {
        IsCleared = true;
    }
    if (IsCleared && IsStage) {
        // 取得視窗中央座標
        ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                               ImGui::GetIO().DisplaySize.y * 0.5f);

        // 設定下個視窗的起始位置為畫面中央（anchor = 0.5, 0.5 表示居中對齊）
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        // 設定背景為白色，文字為橘色
        ImGui::SetNextWindowBgAlpha(1.0f);  // 不透明
        ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(255, 255, 255, 255));  // 白底
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 165, 0, 255));        // 橘字

        ImGui::Begin("CLEAR", nullptr,
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar
        );

        ImGui::SetWindowFontScale(2.5f);
        ImGui::Text("CLEAR!");

        ImGui::End();
        ImGui::PopStyleColor(2);
    }


    // 掉落重置--------------
    for (auto& pico : m_pico) {
        // if (pico -> m_Transform.translation.y < -500) {
        //     float x = pico -> m_Transform.translation.x - 200.0f;
        //     pico -> SetPosition({x, 200.0f});
        //     pico -> SetSpeed(1, -pico -> GetSpeed(1));
        // }
        if (pico -> m_Transform.translation.y < -500) {
            float x = pico -> m_Transform.translation.x - 200.0f, x1 = pico -> m_Transform.translation.x;
            if (m_Phase == Phase::STAGE_THREE ) {
                if (x1>20) {
                    pico -> SetPosition({x1-100, 200.0f});
                }
                else {
                    pico -> SetPosition({30, 200.0f});
                }
            }
            else {
                pico -> SetPosition({x, 200.0f});
            }
            pico -> SetSpeed(1, -pico -> GetSpeed(1));
        }
    }

    m_pico1 -> Ismoving();
    m_pico2 -> Ismoving();


    for (auto obj : m_Objects) {
        obj -> Move();
        obj -> ResetCurrNumber();
        obj -> ResetCollision();
        obj -> ResetCurrPico();
    }

    m_Root.Update();

    // Key 位子測試
    // ImGui::Begin("test");
    // ImGui::SetWindowSize({300, 300});
    // ImGui::DragFloat2("position", &m_key->m_Transform.translation[0],1,-100, 100);
    // ImGui::End();

}
