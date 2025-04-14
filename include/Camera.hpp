#pragma once
#include <glm/glm.hpp>
#include <memory>

class Character; // 前向宣告

class Camera {
public:
    Camera(float viewWidth, float viewHeight);

    float Update(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2);
    void SetBoundaries(float left, float right, float top, float bottom);
    bool IsCharacterOutOfBounds(const std::shared_ptr<Character>& character) const;
    void ForceCharactersInView(const std::shared_ptr<Character>& pico1, const std::shared_ptr<Character>& pico2);

    glm::vec2 WorldToScreenPosition(const glm::vec2& worldPos) const;
    glm::vec2 ScreenToWorldPosition(const glm::vec2& screenPos) const;
    glm::vec2 CheckBoundaries(const glm::vec2& characterPos, const glm::vec2& characterSize) const;

    const glm::vec2& GetPosition() const { return m_Position; }
    float GetViewWidth() const { return m_ViewWidth; }
    float GetViewHeight() const { return m_ViewHeight; }




private:
    float dif_post;
    glm::vec2 m_Position;
    float m_ViewWidth;
    float m_ViewHeight;
    float m_LeftBoundary;
    float m_RightBoundary;
    float m_TopBoundary;
    float m_BottomBoundary;
    float m_SmoothFactor;
};