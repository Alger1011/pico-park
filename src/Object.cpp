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





