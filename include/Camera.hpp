#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>

class Character;
class MapManager;  // 添加前向聲明

class Camera {
public:
    Camera(float viewWidth, float viewHeight);

    // 更新相機位置，並返回相機 X 軸的位移量
    float Update(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2);

    // 強制角色保持在視圖內
    void ForceCharactersInView(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2);

    // 設置地圖邊界
    void SetBoundaries(float left, float right, float top, float bottom);

    // 設置地圖管理器引用
    void SetMapManager(MapManager* mapManager) {
        m_MapManager = mapManager;
    }

    // 座標轉換函數
    glm::vec2 WorldToScreenPosition(const glm::vec2& worldPos) const;
    glm::vec2 ScreenToWorldPosition(const glm::vec2& screenPos) const;

    // 檢查並確保角色在邊界內
    glm::vec2 CheckBoundaries(const glm::vec2& characterPos, const glm::vec2& characterSize) const;

    // 檢查角色是否超出邊界
    bool IsCharacterOutOfBounds(const std::shared_ptr<Character>& character) const;

    // Getters
    const glm::vec2& GetPosition() const { return m_Position; }
    float GetViewWidth() const { return m_ViewWidth; }
    float GetViewHeight() const { return m_ViewHeight; }

private:
    glm::vec2 m_Position;
    float m_ViewWidth;
    float m_ViewHeight;
    float m_LeftBoundary;
    float m_RightBoundary;
    float m_TopBoundary;
    float m_BottomBoundary;
    float m_SmoothFactor;
    float dif_post;  // 相機 X 軸的位移量
    MapManager* m_MapManager;  // 添加地圖管理器指針
};

#endif // CAMERA_HPP