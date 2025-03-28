#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

#include "Util/GameObject.hpp"

class Character : public Util::GameObject {
public:
    explicit Character(const std::string& ImagePath);

    Character(const Character&) = delete;

    Character(Character&&) = delete;

    Character& operator=(const Character&) = delete;

    Character& operator=(Character&&) = delete;

    [[nodiscard]] const std::string& GetImagePath() const { return m_ImagePath; }

    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    void Isjumping();

    void Ismoving();

    void SetImage(const std::string& ImagePath);

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

    // TODO: Implement the collision detection
    [[nodiscard]] bool IfCollides(const std::shared_ptr<Character>& other) const {
        (void) other;
        return false;
    }

    bool IsStanding(const std::shared_ptr<Character>& character) const;



private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }
    int JumpVelocity = 0;
    std::string m_ImagePath;
};


#endif //CHARACTER_HPP