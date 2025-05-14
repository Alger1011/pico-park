#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Character.hpp"
#include <string>
#include <memory>
#include <vector>
#include "Util/GameObject.hpp"
#include "Util/Logger.hpp"

class Character;
// 基礎類別 Object
class Object : public Util::GameObject {
public:
    explicit Object(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);

    void SetImage(const std::string& ImagePath);
    void SetPosition(glm::vec2 position) { m_Transform.translation = position; }
    [[nodiscard]] const std::string& GetImagePath() const { return m_ImagePath; }
    int IfCollidesObj(const std::shared_ptr<Object>& other) const;
    void ObjPositionCorrection(int direction, const std::shared_ptr<Object>& other);

    glm::vec2 GetSize() const { return m_size; }
    glm::vec2 GetPosition() const { return m_Transform.translation; }

    virtual void AddCurrNumber(std::shared_ptr<Character>& character) {};
    virtual void Move() {};
    virtual std::string GetType() { return "Object"; }
    virtual void ResetCurrNumber() {};
    virtual void BindTo(const std::shared_ptr<Character>& character) {};
    virtual bool IsBound() {};
    virtual std::shared_ptr<Character> GetBoundPico() const;

protected:
    std::string m_ImagePath;
    glm::vec2 m_size;
};

// 會上下升降的 Board
class Board : public Object {
public:
    explicit Board(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);

    void AddCurrNumber(std::shared_ptr<Character>& character) override { curr_number.push_back(character); }
    void Move() override;
    std::string GetType() override { return "Board"; }
    void ResetCurrNumber() override { curr_number.clear(); }

private:
    float speed = 5;
    int max_height = 200;
    glm::vec2 origin_position;
    int max_number = 2;
    std::vector<std::shared_ptr<Character>> curr_number;
};

// button
class Button : public Object {
public:
    explicit Button(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);
    void Move() override;
    void AddCurrNumber(std::shared_ptr<Character>& character) override { curr_number.push_back(character); }
    bool Press();
    std::string GetType() override { return "Button"; }
    // void ResetCurrNumber() override { curr_number.clear(); }

private:
    std::vector<std::shared_ptr<Character>> curr_number;
    bool is_press = false;
    glm::vec2 origin_position;
};

// 按按鈕後左右移動的板子
class Platform : public Object {
public:
    explicit Platform(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);
    void Move() override;
    std::string GetType() override { return "Platform"; }
    void SetButton(const std::shared_ptr<Button>& button);

private:
    int moved_distance = 0;
    float speed = 10;
    glm::vec2 origin_position;
    std::shared_ptr<Button> m_Button;
    int max_width = 250;
};

// key
class Key : public Object {
public:
    explicit Key(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);
    void Move() override;
    std::string GetType() override { return "Key"; }
    void BindTo(const std::shared_ptr<Character>& pico) override;
    bool IsBound() override;
    std::shared_ptr<Character> GetBoundPico() const override;
    void SetHoldKey(bool hold_key) {
        Hold_key = hold_key;
    }

private:
    glm::vec2 origin_position;
    std::shared_ptr<Character> m_BoundPico = nullptr;
    bool Hold_key = true;
};

// door
class Door : public Object {
public:
    explicit Door(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);
    // void Move() override;
    std::string GetType() override { return "Door"; }
    // void SetKey(const std::shared_ptr<Key>& key);
    // void BindTo(const std::shared_ptr<Character>& character) override;
    // bool IsBound() override;

private:
    glm::vec2 origin_position;
};

// 可以推動的箱子
class Box : public Object {
public:
    explicit Box(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);

    void AddCurrNumber(std::shared_ptr<Character>& character) override { curr_number.push_back(character); }
    void Move() override;
    std::string GetType() override { return "Box"; }
    void ResetCurrNumber() override { curr_number.clear(); }
    void Push(float force);
    void BoxMoving();

private:
    int max_low = 200;
    glm::vec2 origin_position;
    int max_number = 2;
    std::vector<std::shared_ptr<Character>> curr_number;
    std::vector<float> m_Boxspeed = {0, 0, 0, 0}; //上下左右
    float m_fallSpeed = 5.0f;
};

#endif // OBJECT_HPP