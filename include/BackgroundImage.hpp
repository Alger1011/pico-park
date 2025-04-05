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

    // void NextPhase(const int phase) {
    //     auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
    //     temp->SetImage(ImagePath(phase));
    // }
    void NextPhase(const int phase) {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        // 特別處理 Phase 3，使用地圖渲染
        if (phase ==4) {
            std::vector<std::vector<int>> map = Map::LoadMap(GA_RESOURCE_DIR"/Map/first.txt");
            // 這裡可以進一步實現將地圖轉換為背景圖的邏輯
            temp->SetImage(CreateMapImage(map));
        } else {
            temp->SetImage(ImagePath(phase));
        }
    }
private:
    inline std::string ImagePath(const int phase) {
        return GA_RESOURCE_DIR"/Image/Background/phase" + std::to_string(phase) + ".png";
    }

    // 新增方法：將地圖轉換為背景圖
    std::string CreateMapImage(const std::vector<std::vector<int>>& map) {
        // TODO: 實現地圖到圖像的轉換
        // 可以使用 SDL, OpenGL 或其他圖像庫來動態生成背景
        return GA_RESOURCE_DIR"/Image/Background/phase3.png";  // 暫時返回原本的圖
    }
};

#endif //BACKGROUND_IMAGE_HPP