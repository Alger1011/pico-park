#ifndef OBJECT_HPP
#define CHARACTER_HPP

#include <string>
#include "Util/GameObject.hpp"

class Object : public Util::GameObject {
public:
    explicit Object(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);
    void SetImage(const std::string& ImagePath);




private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }
    std::string m_ImagePath;
    glm::vec2 m_size;
};


#endif //OBJECT_HPP