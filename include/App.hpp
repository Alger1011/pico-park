#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "Util/Text.hpp"
#include "PhaseResourceManger.hpp"
#include "AnimatedCharacter.hpp"
#include "Camera.hpp"
#include "MapManager.hpp"
#include "Object.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    void CreateMapTiles(const std::vector<std::vector<int>>& map);

    bool CheckTileCollision(glm::vec2 charPos, glm::vec2 charSize, glm::vec2& correctedPos, float& velocityY);

    void PassCheck();

    void CreateMapTiles(const std::string& filename);

    void Reset();

private:
    bool IsCleared = false;  // 判斷是否通關
    bool IsStage = false;
    void ValidTask();

    enum class Phase {
        Welcome,
        STAGE_ONE_LOADING,
        STAGE_ONE,
        STAGE_TWO_LOADING,
        STAGE_TWO,
        STAGE_THREE_LOADING,
        STAGE_THREE,
        STAGE_FOUR_LOADING,
        STAGE_FOUR,
    };

    State m_CurrentState = State::START;
    Phase m_Phase = Phase::Welcome;

    Util::Renderer m_Root;
    std::vector<std::vector<int>> m_Map;
    std::vector<std::shared_ptr<Character>> m_MapTiles;

    std::vector<std::shared_ptr<Object>> m_Objects;

    std::shared_ptr<Character> m_pico1;
    std::shared_ptr<Character> m_pico2;

    std::vector<std::shared_ptr<Character>> m_pico;

    std::shared_ptr<Key> m_key;
    std::shared_ptr<Character> m_door1;
    std::shared_ptr<Character> m_board1;

    std::shared_ptr<Character> m_Giraffe;
    std::shared_ptr<Character> m_Chest;
    std::vector<std::shared_ptr<Character>> m_Doors;

    std::shared_ptr<AnimatedCharacter> m_Bee;
    std::shared_ptr<AnimatedCharacter> m_Ball;

    std::shared_ptr<Util::GameObject> gm;
    std::shared_ptr<PhaseResourceManger> m_PRM;

    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<MapManager> m_MapManager;

    bool m_spaceDown = false;
    bool m_EnterDown = false;
    bool m_IsCharacterOutOfBounds = false;
};

#endif