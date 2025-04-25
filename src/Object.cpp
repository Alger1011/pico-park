#include "Object.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include "Util/Keycode.hpp"


Object::Object(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) {
    SetImage(ImagePath);
    m_Transform.translation = position;
    m_size = size;
}

void Object::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

//bool Character::IfCollides(const std::shared_ptr<Character>& other) const {
//    glm::vec2 pos1 = this->GetPosition();
//    glm::vec2 pos2 = other->GetPosition();
//
//    float distance = glm::length(pos1 - pos2);
//    return distance < 50.0f;
//}





