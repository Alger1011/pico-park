#include "Object.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include "Util/Keycode.hpp"

class Object;

Object::Object(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) {
    SetImage(ImagePath);
    m_Transform.translation = position;
    m_size = size;
}

void Object::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

int Object::IfCollidesObj(const std::shared_ptr<Object>& other) const {
    int collision = -1;
    glm::vec2 posA = other -> m_Transform.translation;
    glm::vec2 sizeA = other -> GetSize();

    // 物體 A 的邊界 (左, 右, 上, 下)
    float leftA = posA.x - sizeA.x / 2;
    float rightA = posA.x + sizeA.x / 2;
    float topA = posA.y + sizeA.y / 2;  // 上邊界
    float bottomA = posA.y - sizeA.y / 2;  // 下邊界

    glm::vec2 posB = m_Transform.translation;
    glm::vec2 sizeB = GetSize();

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
    if (collision != -1 && other -> GetType() == "Key") {
        return 4;
    }

    // 沒有碰撞
    return collision;
}

void Object::ObjPositionCorrection(int direction, const std::shared_ptr<Object>& other) {
    if (direction == 0) {
        float posY = other->GetPosition().y + other->GetSize().y/2 + GetSize().y/2 - 1;
        m_Transform.translation.y = posY;
    }
    else if (direction == 1) {
        float posY = other->GetPosition().y - other->GetSize().y/2 - GetSize().y/2;
        m_Transform.translation.y = posY;
    }
    else if (direction == 2) {
        float posX = other->GetPosition().x - other->GetSize().x/2 - GetSize().x/2 + 1;
        m_Transform.translation.x = posX;
    }
    else if (direction == 3) {
        float posX = other->GetPosition().x + other->GetSize().x/2 + GetSize().x/2 - 1;
        m_Transform.translation.x = posX;
    }
}


std::shared_ptr<Character> Object::GetBoundPico() const {
    return nullptr;
}

Board::Board(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
    // max_height = height;
}

void Board::Move() {
    if (curr_number.size() == 0) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/board0.png");
    } else if (curr_number.size() == 1) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/board1.png");
    } else {
        SetImage(GA_RESOURCE_DIR"/Image/Character/board.png");
    }

    if (curr_number.size() == max_number  && m_Transform.translation.y - origin_position.y < max_height) {
        m_Transform.translation.y += speed;
        for (auto pico : curr_number) {
            pico -> m_Transform.translation.y += speed;
        }
    }
    else if (curr_number.size() != max_number && m_Transform.translation.y > origin_position.y){
        m_Transform.translation.y -= speed;
        for (auto pico : curr_number) {
            pico -> m_Transform.translation.y -= speed;
        }
    }
}

Button::Button(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

bool Button::Press() {
    if (curr_number.size() >= 1 && !is_press) {
        is_press = true;
        m_Transform.translation.y -= 5.0f;
    }
    return is_press;
}

void Button::Move() {
    //LOG_ERROR("Press, size={}", curr_number.size());
    Press();
}

Platform::Platform(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void Platform::Move() {
    if (!m_Button) return;
    if (m_Button->Press() && moved_distance < max_width) {
        //LOG_ERROR("MOVING");
        m_Transform.translation.x -= speed;
        moved_distance += speed;
    }
}

void Platform::SetButton(const std::shared_ptr<Button>& button) {
    m_Button = button;
}

Key::Key(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void Key::BindTo(const std::shared_ptr<Character>& character) {
    if (Hold_key || !IsBound()) {
        if (m_BoundPico != character) {
            m_BoundPico = character;
            Hold_key = false;
        }
    }
}

bool Key::IsBound() {
    return m_BoundPico != nullptr;
}

std::shared_ptr<Character> Key::GetBoundPico() const {
    return m_BoundPico;
}

void Key::Move() {
    if (m_BoundPico) {
        glm::vec2 picoPos = m_BoundPico->GetPosition();
        glm::vec2 offset = {15.0f, 5.0f};  // 可以根據 key 的外觀調整這個偏移
        SetPosition(picoPos + offset);
    }
}

Door::Door(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

Box::Box(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void Box::Move() {
    if (curr_number.size() == 0) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/long0.png");
    } else if (curr_number.size() == 1) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/long1.png");
    } else {
        SetImage(GA_RESOURCE_DIR"/Image/Character/long2.png");
    }

    // m_Transform.translation.y -= m_fallSpeed;
    // if (m_Transform.translation.y < -150) {
    //     m_Transform.translation.y = -100;
    //     m_fallSpeed = 0;
    // }
}

void Box::Push(float force) {
    m_Transform.translation.x += force;
    // glm::vec2 oldPos = m_Transform.translation;
}

void Box::BoxMoving() {
    m_Transform.translation.y += m_Boxspeed[0];
    m_Transform.translation.y -= m_Boxspeed[1];
    m_Transform.translation.x -= m_Boxspeed[2];
    m_Transform.translation.x += m_Boxspeed[3];
    m_Boxspeed[0] = std::max(m_Boxspeed[0] - m_Boxspeed[1], 0.0f);
    // m_speed[1] = 0;
    m_Boxspeed[2] = 0;
    m_Boxspeed[3] = 0;

}


// void Door::SetKey(const std::shared_ptr<Key>& key) {
//     m_Key = key;
// }
//
// void Door::BindTo(const std::shared_ptr<Character>& character) {
//     if (!IsBound()) m_BoundPico = character;
// }
//
// bool Door::IsBound() {
//     return m_BoundPico != nullptr;
// }


//bool Character::IfCollides(const std::shared_ptr<Character>& other) const {
//    glm::vec2 pos1 = this->GetPosition();
//    glm::vec2 pos2 = other->GetPosition();
//
//    float distance = glm::length(pos1 - pos2);
//    return distance < 50.0f;
//}





