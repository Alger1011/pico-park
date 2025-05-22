#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Map.hpp"

class BackgroundImage : public Util::GameObject {

public:
    BackgroundImage() : GameObject(
            std::make_unique<Util::Image>(GA_RESOURCE_DIR"/Image/Background/Welcome.png"), -10) {
    }

    void NextPhase(const int phase) {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        temp->SetImage(ImagePath(phase));
        // auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        //
        // if (!temp) {
        //     LOG_ERROR("Failed to cast m_Drawable to Util::Image (phase {})", phase);
        //     return;
        // }
        //
        // std::string path = ImagePath(phase);
        //
        // // 確認檔案是否存在
        // if (!std::filesystem::exists(path)) {
        //     LOG_ERROR("Background image not found: {}", path);
        //     return;
        // }
        //
        // LOG_INFO("Setting background image to {}", path);
        // temp->SetImage(path);  // 如果 SetImage() 處理不了壞圖，也可能 crash
    }

private:
    inline std::string ImagePath(const int phase) {
        return GA_RESOURCE_DIR"/Image/Background/phase" + std::to_string(phase) + ".png";
    }

    // 新增方法：將地圖轉換為背景圖
    std::string CreateMapImage(const std::vector<std::vector<int>>& map) {
        // TODO: 實現地圖到圖像的轉換
        // 可以使用 SDL, OpenGL 或其他圖像庫來動態生成背景
        return GA_RESOURCE_DIR"/Image/Background/phase2.png";  // 暫時返回原本的圖
    }
};

#endif //BACKGROUND_IMAGE_HPP