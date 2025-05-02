#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Character.hpp"
#include <string>
#include <memory>
#include <vector>
#include "Util/GameObject.hpp"

class Character;
// 基礎類別 Object
class Object : public Util::GameObject {
public:
    explicit Object(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);

    void SetImage(const std::string& ImagePath);
    void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    glm::vec2 GetSize() const { return m_size; }
    glm::vec2 GetPosition() const { return m_Transform.translation; }

    virtual void AddCurrNumber(std::shared_ptr<Character>& character) {};
    virtual void Move() {};
    virtual std::string GetType() { return "Object"; }
    virtual void ResetCurrNumber() {};

protected:
    std::string m_ImagePath;
    glm::vec2 m_size;
};

// 派生類別 Board
class Board : public Object {
public:
    explicit Board(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);

    void AddCurrNumber(std::shared_ptr<Character>& character) override { curr_number.push_back(character); }
    void Move() override;
    std::string GetType() override { return "Board"; }
    void ResetCurrNumber() override { curr_number.clear(); }

private:
    float speed = 10;
    int max_height = 200;
    glm::vec2 origin_position;
    int max_number = 2;
    std::vector<std::shared_ptr<Character>> curr_number;
};

#endif // OBJECT_HPP