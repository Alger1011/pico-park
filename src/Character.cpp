#include "Character.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
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

void Character::Isjumping() {
    JumpVelocity = 20.0f;
}

void Character::Ismoving() {
    m_Transform.translation.y += JumpVelocity;
    JumpVelocity -= 0.01f;
}

int Character::IfCollidesObject(const std::shared_ptr<Object>& other) const {
    // object position and size, keep return the position, keep collides,
    // where they collides, return the direction
    // 單純偵測碰撞
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




