#include "Character.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include <algorithm>
#include "Util/Keycode.hpp"

Character::Character(const std::string& ImagePath) {
    SetImage(ImagePath);

    ResetPosition();
}

void Character::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;

    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

bool Character::IfCollides(const std::shared_ptr<Character>& other) const {
    glm::vec2 pos1 = this->GetPosition();
    glm::vec2 pos2 = other->GetPosition();

    float distance = glm::length(pos1 - pos2);
    return distance < 50.0f;
}

void Character::Ismoving() {
    m_Transform.translation.y += m_speed[0];
    m_Transform.translation.y -= m_speed[1];
    m_Transform.translation.x -= m_speed[2];
    m_Transform.translation.x += m_speed[3];
    m_speed[0] = std::max(m_speed[0] - m_speed[1], 0.0f);
    // m_speed[1] = 0;
    m_speed[2] = 0;
    m_speed[3] = 0;
}

int Character::IfCollidesObject(const std::shared_ptr<Object>& other) const {
    int collision = -1;
    glm::vec2 posA = other->m_Transform.translation;
    glm::vec2 sizeA = other->GetSize();

    // 物體 A 的邊界 (左, 右, 上, 下)
    float leftA = posA.x - sizeA.x / 2;
    float rightA = posA.x + sizeA.x / 2;
    float topA = posA.y + sizeA.y / 2;  // 上邊界
    float bottomA = posA.y - sizeA.y / 2;  // 下邊界

    glm::vec2 posB = m_Transform.translation;
    glm::vec2 sizeB = Cha_size;

    // 物體 B 的邊界 (左, 右, 上, 下)
    float leftB = posB.x - sizeB.x / 2;
    float rightB = posB.x + sizeB.x / 2;
    float topB = posB.y + sizeB.y / 2;  // 上邊界
    float bottomB = posB.y - sizeB.y / 2;  // 下邊界

    // 檢查碰撞邏輯 (上下左右)
    bool isCollided = !(rightB <= leftA || leftB >= rightA || topB <= bottomA || bottomB >= topA);

    if (isCollided) {
        // 當碰撞發生時，根據物體 B 的位置來處理不同的情況

        // 判斷上下碰撞：
        if (posB.y > posA.y) {
            // 物體 B 在物體 A 之下
            // 如果物體 B 的下邊界 > 物體 A 的上邊界，則發生碰撞
            if (bottomB < topA && (posB.x < rightA && posB.x > leftA)){
                // 物體 B 往上移動的碰撞反應
                collision = 0;
            }
        } else if (posB.y < posA.y) {
            // 物體 B 在物體 A 之上
            if (topB > bottomA && (posB.x < rightA && posB.x > leftA)) {
                // 物體 B 往下移動的碰撞反應
                collision = 1;
            }
        }

        // 判斷左右碰撞：
        if (posB.x > posA.x) {
            // 物體 B 在物體 A 的右邊
            if (leftB < rightA && (posB.y <= topA && posB.y >= bottomA)) {
                // 物體 B 往左移動的碰撞反應
                collision = 3;
            }
        } else if (posB.x < posA.x) {
            // 物體 B 在物體 A 的左邊
            if (rightB > leftA && (posB.y <= topA && posB.y >= bottomA)) {
                // 物體 B 往右移動的碰撞反應
                collision = 2;
            }
        }
    }
    if (collision != -1 && other -> GetType() == "Door") {
        return 4;
    }

    // 沒有碰撞
    return collision;
}


void Character::SetSpeed(int direction, float speed){
    m_speed[direction] += speed;
}

bool Character::IsStanding(const std::shared_ptr<Character>& other) const {
    glm::vec2 pos1 = this->GetPosition();
    glm::vec2 pos1_size = this->GetSize();   // 用這個來抓寬高
    glm::vec2 pos2 = other->GetPosition();
    glm::vec2 pos2_size = other->GetSize(); // 抓 B 的寬高
    float myFeetY = pos1.y + pos1_size.y;
    float otherHeadY = pos2.y;
    // 1. 垂直方向：A 的腳 ≒ B 的頭
    bool verticalAligned = myFeetY >= otherHeadY - 15.0f && myFeetY <= otherHeadY + 15.0f;

    // 2. 水平方向：A 的腳範圍要壓在線上 B 的頭部
    bool horizontalAligned =
        pos1.x + pos1_size.x > pos2.x &&
        pos1.x < pos2.x + pos2_size.x;

    return verticalAligned && horizontalAligned;
}

bool Character::IsStandingOnBoard(const std::shared_ptr<Character>& board) const {
    glm::vec2 myPos = this->GetPosition();
    glm::vec2 mySize = this->GetSize();

    glm::vec2 boardPos = board->GetPosition();
    glm::vec2 boardSize = board->GetSize();

    float myFeetY = myPos.y + mySize.y;
    float boardTopY = boardPos.y;

    bool verticalAligned = myFeetY >= boardTopY - 15.0f && myFeetY <= boardTopY + 15.0f;

    float myCenterX = myPos.x + mySize.x / 2.0f;
    float boardLineStartX = boardPos.x + boardSize.x * 0.2f;
    float boardLineEndX = boardPos.x + boardSize.x * 0.8f;

    bool horizontalAligned = myCenterX >= boardLineStartX && myCenterX <= boardLineEndX;
    return verticalAligned && horizontalAligned;
}

void Character::PositionCorrection(int direction, const std::shared_ptr<Object>& other) {
    if (direction == 0) {
        float posY = other->GetPosition().y + other->GetSize().y/2 + Cha_size.y/2 - 1;
        m_Transform.translation.y = posY;
    }
    else if (direction == 1) {
        float posY = other->GetPosition().y - other->GetSize().y/2 - Cha_size.y/2;
        m_Transform.translation.y = posY;
    }
    else if (direction == 2) {
        float posX = other->GetPosition().x - other->GetSize().x/2 - Cha_size.x/2 + 1;
        m_Transform.translation.x = posX;
    }
    else if (direction == 3) {
        float posX = other->GetPosition().x + other->GetSize().x/2 + Cha_size.x/2 - 1;
        m_Transform.translation.x = posX;
    }
}

void Character::HeadCorrection(const std::shared_ptr<Character>& other) {
    m_Transform.translation.y = other->GetPosition().y + other->GetSize().y/2 + Cha_size.y/2 ;
}

int Character::IfCollidesCha(const std::shared_ptr<Character>& other) const {
    glm::vec2 posA = other->m_Transform.translation;
    glm::vec2 sizeA = other->GetSize();

    // 物體 A 的邊界 (左, 右, 上, 下)
    float leftA = posA.x - sizeA.x / 2;
    float rightA = posA.x + sizeA.x / 2;
    float topA = posA.y + sizeA.y / 2;  // 上邊界
    float bottomA = posA.y - sizeA.y / 2;  // 下邊界

    glm::vec2 posB = m_Transform.translation;
    glm::vec2 sizeB = Cha_size;

    // 物體 B 的邊界 (左, 右, 上, 下)
    float leftB = posB.x - sizeB.x / 2;
    float rightB = posB.x + sizeB.x / 2;
    float topB = posB.y + sizeB.y / 2;  // 上邊界
    float bottomB = posB.y - sizeB.y / 2;  // 下邊界

    // 檢查碰撞邏輯 (上下左右)
    bool isCollided = !(rightB <= leftA || leftB >= rightA || topB <= bottomA || bottomB >= topA);

    if (isCollided) {
        // 當碰撞發生時，根據物體 B 的位置來處理不同的情況

        // 判斷上下碰撞：
        if (posB.y > posA.y) {
            // 物體 B 在物體 A 之下
            // 如果物體 B 的下邊界 > 物體 A 的上邊界，則發生碰撞
            if (bottomB < topA && (posB.x < rightA && posB.x > leftA)){
                // 物體 B 往上移動的碰撞反應
                return 0;
            }
        } else if (posB.y < posA.y) {
            // 物體 B 在物體 A 之上
            if (topB > bottomA && (posB.x < rightA && posB.x > leftA)) {
                // 物體 B 往下移動的碰撞反應
                return 1;
            }
        }

        // 判斷左右碰撞：
        if (posB.x > posA.x) {
            // 物體 B 在物體 A 的右邊
            if (leftB < rightA && (posB.y <= topA && posB.y >= bottomA)) {
                // 物體 B 往左移動的碰撞反應
                return 3;
            }
        } else if (posB.x < posA.x) {
            // 物體 B 在物體 A 的左邊
            if (rightB > leftA && (posB.y <= topA && posB.y >= bottomA)) {
                // 物體 B 往右移動的碰撞反應
                return 2;
            }
        }
    }
    // 沒有碰撞
    return -1;
}

void Character::ChaPositionCorrection(int direction, const std::shared_ptr<Character>& other) {
    if (direction == 0) {
        float posY = other->GetPosition().y + other->GetSize().y/2 + Cha_size.y/2 - 1;
        m_Transform.translation.y = posY;
    }
    else if (direction == 1) {
        float posY = other->GetPosition().y - other->GetSize().y/2 - Cha_size.y/2;
        m_Transform.translation.y = posY;
    }
    else if (direction == 2) {
        float posX = other->GetPosition().x - other->GetSize().x/2 - Cha_size.x/2 + 1;
        m_Transform.translation.x = posX;
    }
    else if (direction == 3) {
        float posX = other->GetPosition().x + other->GetSize().x/2 + Cha_size.x/2 - 1;
        m_Transform.translation.x = posX;
    }
}




