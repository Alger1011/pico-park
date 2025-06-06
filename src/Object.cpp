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

void Object::SetSpeed(int direction, float speed){
    m_speed[direction] += speed;
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
        SetImage(GA_RESOURCE_DIR"/Image/Character/board.png");
    } else if (curr_number.size() == 1) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/board1.png");
    } else {
        SetImage(GA_RESOURCE_DIR"/Image/Character/board0.png");
    }

    if (curr_number.size() == max_number && m_Transform.translation.y - origin_position.y < max_height) {
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
    Press();
}

Platform::Platform(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void Platform::Move() {
    if (!m_Button) return;
    if (m_Button->Press() && moved_distance < max_width) {
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
        SetImage(GA_RESOURCE_DIR"/Image/Character/long2.png");
    } else if (curr_number.size() == 1) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/long1.png");
    } else {
        SetImage(GA_RESOURCE_DIR"/Image/Character/long0.png");
    }

    if (m_Transform.translation.y > max_low) {
        m_Transform.translation.y -= 5.0f;
    }
    if (isFall == true && dropped == false) {
        m_Transform.translation.x += 22.0f;
        dropped = true;
    }
    if (curr_number.size() == max_number && isFall == false) {
        if (force > 0 && If_Collision[3] == 0) {
            speed[1] += force;
            for (auto pico : curr_number) {
                pico -> m_Transform.translation.x += force;
            }
        }
        else if (force < 0 && If_Collision[2] == 0) {
            speed[0] += force;
        // m_Transform.translation.x += force;
            for (auto pico : curr_number) {
                pico -> m_Transform.translation.x += force;
            }
        }
    }
    if (If_Collision[3] != 0) {
        speed[1] = 0;
    }
    if (If_Collision[2] != 0) {
        speed[0] = 0;
    }
    m_Transform.translation.x += speed[0];
    m_Transform.translation.x += speed[1];
    speed = {0, 0};
    isFall = true;
}

void Box::Push(float F) {
    force = F;
}


Square_Box::Square_Box(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void Square_Box::Move() {
    if (curr_number.size() == 0) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/square1.png");
    } else if (curr_number.size() >= 1) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/square0.png");
    } else {
        SetImage(GA_RESOURCE_DIR"/Image/Character/square1.png");
    }
    if (curr_number.size() >= max_number) {
        if (force >= 0 && If_Collision[3] == 0) {
            speed[1] += force;
            for (auto pico : curr_number) {
                pico -> m_Transform.translation.x += force;
            }
        }
        else if (force <= 0 && If_Collision[2] == 0) {
            speed[0] += force;
            for (auto pico : curr_number) {
                pico -> m_Transform.translation.x += force;
            }
        }
        // m_Transform.translation.x += force;
    }
    if (If_Collision[3] != 0) {
        speed[1] = 0;
    }
    if (If_Collision[2] != 0) {
        speed[0] = 0;
    }
    m_Transform.translation.x += speed[0];
    m_Transform.translation.x += speed[1];
    speed = {0, 0};

}

void Small_Box::Push(float F) {
    force = F;
}

Small_Box::Small_Box(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void Small_Box::Move() {
    if (curr_number.size() + curr_pico == 0) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/brick.png");
    } else if (curr_number.size() + curr_pico >= 1) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/brick0.png");
    } else {
        SetImage(GA_RESOURCE_DIR"/Image/Character/brick.png");
    }

    if (curr_number.size() + curr_pico >= max_number) {
        if (force >= 0 && If_Collision[3] == 0){
             // m_Transform.translation.x += force;
            speed[1] += force;
            for (auto pico : curr_number) {
                pico -> m_Transform.translation.x += force;
            }
        }
        else if (force <= 0 && If_Collision[2] == 0) {
            // m_Transform.translation.x += force;
            speed[0] += force;
            for (auto pico : curr_number) {
                pico -> m_Transform.translation.x += force;
            }
        }
        //
        // m_Transform.translation.x += force;
        // for (auto pico : curr_number) {
        //     pico -> m_Transform.translation.x += force;
        // }

        if (If_Collision[3] != 0) {
            speed[1] = 0;
        }
        else if (If_Collision[2] != 0) {
            speed[0] = 0;
        }

    }
    if (isFall == true && If_Collision[1] == 0) {
        m_Transform.translation.y -= 7.0f;
    }
    if (m_Transform.translation.y < max_low) {
        SetPosition({origin_position.x, 200});
        speed = {0, 0};
    }

    // if (StandOnPico != nullptr) {
    //     m_Transform.translation.x -= speed[0];
    //     m_Transform.translation.x += speed[1];
    // }
    m_Transform.translation.x += speed[0];
    m_Transform.translation.x += speed[1];
    speed = {0, 0};
    isFall = true;
    force = 0;
}

void Square_Box::Push(float F) {
    force = F;
}

Platform1::Platform1(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void Platform1::Move() {
    if (!m_Button) return;
    if (m_Button -> Press() && moved_distance < max_width) {
        m_Transform.translation.x -= speed;
        moved_distance += speed;
    }
}

void Platform1::SetButton(const std::shared_ptr<Button>& button) {
    m_Button = button;
}


Platform2::Platform2(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void Platform2::Move() {
    if (!m_Button) return;
    if (m_Button -> Press() && moved_distance < max_width) {
        m_Transform.translation.y -= speed;
        moved_distance += speed;
    }
}

void Platform2::SetButton(const std::shared_ptr<Button>& button) {
    m_Button = button;
}

Button1::Button1(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}


void Button1::Move() {
    if (curr_number.size() + curr_pico >= 1 && origin_position.y - m_Transform.translation.y < max_low) {
        m_Transform.translation.y -= 5.0f;
        is_press = true;
    }
    else if (curr_number.size() + curr_pico == 0 && m_Transform.translation.y - origin_position.y < max_height) {
        m_Transform.translation.y += 5.0f;
        is_press = false;
    }
}

bool Button1::Press() {
    return is_press;
}

void Platform3::SetButton(const std::shared_ptr<Button1>& button) {
    m_Button = button;
}


Platform3::Platform3(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void Platform3::Move() {
    if (!m_Button) return;
    if (m_Button -> Press() && m_Transform.translation.y - origin_position.y < max) {
        m_Transform.translation.y += speed;
    }
    else if (!m_Button -> Press() && m_Transform.translation.y > origin_position.y) {
        m_Transform.translation.y -= speed;
    }
}

AutoBoard::AutoBoard(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void AutoBoard::Move() {
    if (going_up) {
        m_Transform.translation.y += speed;
        for (auto pico : curr_number) {
            pico->m_Transform.translation.y += speed;
        }

        if (m_Transform.translation.y - origin_position.y > max_height) {
            going_up = false;
        }
    } else {
        m_Transform.translation.y -= speed;
        for (auto pico : curr_number) {
            pico->m_Transform.translation.y -= speed;
        }

        if (m_Transform.translation.y < origin_position.y) {
            going_up = true;
        }
    }
}

AutoBoard1::AutoBoard1(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void AutoBoard1::Move() {
    if (going_right) {
        m_Transform.translation.x += speed;
        moved_distance += speed;
        for (auto pico : curr_number) {
            pico->m_Transform.translation.x += speed;
        }

        if (moved_distance > max_right) {
            going_right = false;
            moved_distance = 0;
        }
    } else {
        m_Transform.translation.x -= speed;
        moved_distance += speed;
        for (auto pico : curr_number) {
            pico->m_Transform.translation.x -= speed;
        }

        if (moved_distance > max_right) {
            going_right = true;
            moved_distance = 0;
        }
    }
}

Board1::Board1(const std::string& ImagePath, glm::vec2 position, glm::vec2 size) : Object(ImagePath, position, size)
{
    origin_position = position;
}

void Board1::Move() {
    if (curr_number.size() == 0) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/board.png");
    } else if (curr_number.size() == 1) {
        SetImage(GA_RESOURCE_DIR"/Image/Character/board1.png");
    } else {
        SetImage(GA_RESOURCE_DIR"/Image/Character/board0.png");
    }

    if (curr_number.size() == max_number && origin_position.y - m_Transform.translation.y < max_height) {
        m_Transform.translation.y -= speed;
        for (auto pico : curr_number) {
            pico -> m_Transform.translation.y -= speed;
        }
    }
    else if (curr_number.size() != max_number && m_Transform.translation.y < origin_position.y){
        m_Transform.translation.y += speed;
        for (auto pico : curr_number) {
            pico -> m_Transform.translation.y += speed;
        }
    }
}
