#include "Renderer.hpp"
#include "Util/GameObject.hpp"
#include "Util/Logger.hpp"

Renderer::Renderer(const std::shared_ptr<Camera>& camera,
                   const std::vector<std::shared_ptr<Util::GameObject>>& children)
    : m_Camera(camera)
    , m_ViewWidth(1000.0f)   // 左右各 500 單位
    , m_ViewHeight(562.0f)   // 上下各 281 單位
{
    if (!children.empty()) {
        m_Renderer.AddChildren(children);
    }

    if (m_Camera) {
        LOG_INFO("Renderer initialized with camera");
    } else {
        LOG_INFO("Renderer initialized without camera");
    }
}

void Renderer::SetCamera(const std::shared_ptr<Camera>& camera) {
    m_Camera = camera;
    LOG_INFO("Camera set on renderer");
}

std::shared_ptr<Camera> Renderer::GetCamera() const {
    return m_Camera;
}

void Renderer::Update() {
    if (!m_Camera) {
        // 如果沒有相機，則正常渲染所有物體
        m_Renderer.Update();
        return;
    }

    // 由於我們無法直接獲取 m_Renderer 中的對象，
    // 我們需要修改策略：
    // 1. 創建一個可見物體的列表
    // 2. 暫時移除不可見的物體
    // 3. 更新渲染器
    // 4. 恢復所有物體

    // 這需要修改 Util::Renderer 的實現，或者改變我們的方法
    // 此處，我們將簡單地修改為：在 Update 之前透過另一種方式檢查可見性
    m_Renderer.Update();

    // 注意：由於我們無法訪問 Util::Renderer 的內部物體，
    // 這裡的實現只是一個佔位符，實際上並未實現視野裁剪
    LOG_INFO("Rendering with camera at position ({}, {})",
             m_Camera->GetPosition().x, m_Camera->GetPosition().y);
}

bool Renderer::IsObjectVisible(const std::shared_ptr<Util::GameObject>& object) const {
    if (!m_Camera || !object) {
        return false;
    }

    // 由於 GameObject 沒有 GetPosition() 和 GetSize() 方法，
    // 我們需要使用其他方法來獲取位置和大小

    // 假設 GameObject 有某種方式可以判斷位置
    // 這裡只是一個佔位符，實際實現需要根據 GameObject 的接口來調整

    // 由於缺乏必要的訪問方法，此處應該按照實際的 GameObject 接口進行修改
    return true; // 暫時默認所有物體可見
}