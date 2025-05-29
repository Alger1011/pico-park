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
    void SetSpeed (int direction, float speed);
    float GetSpeed(int direction) {return m_speed[direction];}
    void AddCollision(int direction) { If_Collision[direction] += 1; }
    void ResetCollision() { If_Collision = {0,0,0,0};};
    int GetCollision(const int direction) { return If_Collision[direction]; };


    glm::vec2 GetSize() const { return m_size; }
    glm::vec2 GetPosition() const { return m_Transform.translation; }

    virtual void AddCurrNumber(std::shared_ptr<Character>& character) {};
    virtual std::vector<std::shared_ptr<Character>> GetCurrNumber() {};
    virtual void Move() {};
    virtual std::string GetType() { return "Object"; }
    virtual void ResetCurrNumber() {};
    virtual void BindTo(const std::shared_ptr<Character>& character) {};
    virtual bool IsBound() {};
    virtual std::shared_ptr<Character> GetBoundPico() const;
    virtual void Push(float F) {};
    virtual void SetFall(bool fall) {};
    virtual void StandOnCharacter(std::shared_ptr<Character>& character) {};
    virtual void GetPicoSpeed(float speed1, float speed2) {};
    virtual std::shared_ptr<Character> StandOn() {};
    virtual void AddCurrPico() {};
    virtual int GetCurrPico() {};
    virtual void ResetCurrPico() {};


protected:
    std::string m_ImagePath;
    glm::vec2 m_size;
    std::vector<float> m_speed = {0, 0, 0, 0}; //上下左右
    std::vector<float> If_Collision = {0, 0, 0, 0}; //上下左右的碰撞
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
    int max_height = 210;
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
    std::string GetType() override { return "Door"; }

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
    void Push(float F) override;
    void SetFall(bool fall) override { isFall = fall; };

private:
    float max_low = -350;
    glm::vec2 origin_position;
    int max_number = 2;
    std::vector<std::shared_ptr<Character>> curr_number;
    bool isFall = true;
    float force = 0;
    bool dropped = false;
    std::vector<float> speed = {0, 0}; // 左右速度
};

// 可以推動的正方形箱子
class Square_Box : public Object {
public:
    explicit Square_Box(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);

    void AddCurrNumber(std::shared_ptr<Character>& character) override { curr_number.push_back(character); }
    void Move() override;
    std::string GetType() override { return "Square_Box"; }
    void ResetCurrNumber() override { curr_number.clear(); }
    void Push(float F) override;

private:
    glm::vec2 origin_position;
    int max_number = 1;
    std::vector<std::shared_ptr<Character>> curr_number;
    float force = 0;
};

// 第三關的小箱子
class Small_Box : public Object {
public:
    explicit Small_Box(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);

    void AddCurrNumber(std::shared_ptr<Character>& character) override { curr_number.push_back(character); }
    void Move() override;
    std::string GetType() override { return "Small_Box"; }
    void ResetCurrNumber() override { curr_number.clear(); }
    void Push(float F) override;
    void SetFall(bool fall) override { isFall = fall; };
    void StandOnCharacter(std::shared_ptr<Character>& character) override { StandOnPico = character; };
    void GetPicoSpeed(float speed1, float speed2) override { speed = { speed1, speed2 }; };
    std::shared_ptr<Character> StandOn() override { return StandOnPico; };
    std::vector<std::shared_ptr<Character>> GetCurrNumber() override { return curr_number;};
    void AddCurrPico() override { curr_pico += 1;};
    int GetCurrPico() override { return curr_pico; };
    void ResetCurrPico() override { curr_pico = 0; };

private:
    glm::vec2 origin_position;
    int max_number = 1;
    std::vector<std::shared_ptr<Character>> curr_number;
    float force = 0;
    bool isFall = true;
    float max_low = -500;
    std::shared_ptr<Character> StandOnPico;
    std::vector<float> speed = {0, 0}; // 左右速度
    int curr_pico = 0;
};

// 按按鈕後左右移動的板子(rec1)
class Platform1 : public Object {
public:
    explicit Platform1(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);
    void Move() override;
    std::string GetType() override { return "Platform1"; }
    void SetButton(const std::shared_ptr<Button>& button);

private:
    int moved_distance = 0;
    float speed = 10;
    glm::vec2 origin_position;
    std::shared_ptr<Button> m_Button;
    int max_width = 650;
};

// 按按鈕後移動的板子(rec2)
class Platform2 : public Object {
public:
    explicit Platform2(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);
    void Move() override;
    std::string GetType() override { return "Platform2"; }
    void SetButton(const std::shared_ptr<Button>& button);

private:
    int moved_distance = 0;
    float speed = 10;
    glm::vec2 origin_position;
    std::shared_ptr<Button> m_Button;
    int max_width = 200;
};

// 給 small_box 壓的 button
class Button1 : public Object {
public:
    explicit Button1(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);
    void Move() override;
    void AddCurrNumber(std::shared_ptr<Character>& character) override { curr_number.push_back(character); }
    std::string GetType() override { return "Button1"; }
    void AddCurrPico() override { curr_pico += 1;};
    int GetCurrPico() override { return curr_pico; };
    void ResetCurrPico() override { curr_pico = 0; };
    void ResetCurrNumber() override { curr_number.clear(); }
    bool Press();

private:
    std::vector<std::shared_ptr<Character>> curr_number;
    float max_height = 5;
    float max_low = 5;
    glm::vec2 origin_position;
    int curr_pico = 0;
    bool is_press = false;
};

// 按按鈕後移動的板子(rec4, 5, 6)
class Platform3 : public Object {
public:
    explicit Platform3(const std::string& ImagePath, glm::vec2 position, glm::vec2 size);
    void Move() override;
    std::string GetType() override { return "Platform3"; }
    void SetButton(const std::shared_ptr<Button1>& button);

private:
    float speed = 10;
    glm::vec2 origin_position;
    std::shared_ptr<Button1> m_Button;
    float max = 200;
};
#endif // OBJECT_HPP