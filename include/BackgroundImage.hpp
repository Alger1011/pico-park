#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <unordered_set>

class BackgroundImage : public Util::GameObject {

public:
    BackgroundImage() : GameObject(
            std::make_unique<Util::Image>(GA_RESOURCE_DIR"/Image/Background/Welcome.png"), -10) {
    }

    void NextPhase(const int phase) {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        temp->SetImage(ImagePath(phase));
    }

private:
    inline std::string ImagePath(const int phase) {
        // static const std::unordered_set<int> skipPhases = {3, 5, 7};
        // if (skipPhases.count(phase)) {
        //      return ""; // 表示該 phase 不顯示圖片
        // }
        // else {
        //     return GA_RESOURCE_DIR"/Image/Background/phase" + std::to_string(phase) + ".png";
        // }
        return GA_RESOURCE_DIR"/Image/Background/phase" + std::to_string(phase) + ".png";
    }
};

#endif //BACKGROUND_IMAGE_HPP
