#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "Object.hpp"
#include "Util/GameObject.hpp"

class Camera;
class Object;

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

    [[nodiscard]] glm::vec2 GetSize() const { return Cha_size; }

    void Ismoving();

    void SetImage(const std::string& ImagePath);

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

    bool IfCollides(const std::shared_ptr<Character>& other) const;

    int IfCollidesObject(const std::shared_ptr<Object>& other) const;

    int IfCollidesCha(const std::shared_ptr<Character>& other) const;

    void SetSpeed(int direction, float speed);

    bool IsStanding(const std::shared_ptr<Character>& character) const;

    bool IsStandingOnBoard(const std::shared_ptr<Character>& character) const;

    float GetSpeed(int direction) {return m_speed[direction];}

    void PositionCorrection(int direction, const std::shared_ptr<Object>& other);

    void ChaPositionCorrection(int direction, const std::shared_ptr<Character>& other);

    void HeadCorrection(const std::shared_ptr<Character>& other);

    void SetJumpState(bool state) { CanJump = state; }

    bool GetJumpState() { return CanJump; }

    void SetOnHead(bool state) { OnHead = state; }

    bool GetOnHead() { return OnHead; };

    glm::vec2 GetScaledSize() const {
        // 假設有一個原始大小
        glm::vec2 originalSize(100.0f, 100.0f); // 根據實際情況調整
        return originalSize * m_Transform.scale;
    }

    bool CheckTileCollision();


private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }
    std::string m_ImagePath;
    glm::vec2 Cha_size = {50, 50};
    std::vector<float> m_speed = {0, 0, 0, 0}; //上下左右
    bool CanJump = false;
    bool OnHead = false;
};


#endif //CHARACTER_HPP