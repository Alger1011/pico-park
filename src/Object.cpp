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

Board::Board(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
    // max_height = height;
}

void Board::Move() {
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


//bool Character::IfCollides(const std::shared_ptr<Character>& other) const {
//    glm::vec2 pos1 = this->GetPosition();
//    glm::vec2 pos2 = other->GetPosition();
//
//    float distance = glm::length(pos1 - pos2);
//    return distance < 50.0f;
//}





