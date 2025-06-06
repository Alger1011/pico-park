#ifndef Renderer_HPP
#define Renderer_HPP

#include "Util/Renderer.hpp"
#include "Util/GameObject.hpp"
#include "Camera.hpp"
#include <memory>

/**
 * @class Renderer
 * @brief A class handling GameObjects' Draw() with camera-based view frustum culling
 * @see Util::GameObject
 * @see Camera
 */
class Renderer final {
public:
    /**
     * @brief Parameterized constructor.
     *
     * @param camera The camera used for frustum culling
     * @param children The GameObject needing to be managed by Renderer.
     */
    Renderer(const std::shared_ptr<Camera>& camera = nullptr,
             const std::vector<std::shared_ptr<Util::GameObject>>& children = {});

    /**
     * @brief Set the camera used for frustum culling.
     *
     * @param camera The camera to use.
     */
    void SetCamera(const std::shared_ptr<Camera>& camera);

    /**
     * @brief Get the current camera.
     *
     * @return The current camera.
     */
    std::shared_ptr<Camera> GetCamera() const;

    /**
     * @brief Add a child to Renderer.
     *
     * @param child The GameObject needing to be managed by Renderer.
     */
    void AddChild(const std::shared_ptr<Util::GameObject>& child) {
        m_Renderer.AddChild(child);
    }

    /**
     * @brief Add children to Renderer.
     *
     * @param children The GameObjects needing to be managed by Renderer.
     */
    void AddChildren(const std::vector<std::shared_ptr<Util::GameObject>>& children) {
        m_Renderer.AddChildren(children);
    }

    /**
     * @brief Remove the child.
     *
     * @param child The GameObject being removed.
     */
    void RemoveChild(std::shared_ptr<Util::GameObject> child) {
        m_Renderer.RemoveChild(child);
    }


    /**
     * @brief Draw children according to their z-index if they are within the camera view.
     *
     * Only objects within the camera's view frustum will be rendered.
     */
    void Update();

private:
    /**
     * @brief Check if a GameObject is visible within the camera view.
     *
     * @param object The GameObject to check.
     * @return True if the object is visible, false otherwise.
     */
    bool IsObjectVisible(const std::shared_ptr<Util::GameObject>& object) const;

    Util::Renderer m_Renderer;
    std::shared_ptr<Camera> m_Camera;
    float m_ViewWidth;   // Camera view width (1000 units total, 500 each side)
    float m_ViewHeight;  // Camera view height (562 units total, 281 each side)
};

#endif