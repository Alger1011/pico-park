#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "Util/Text.hpp"
#include "PhaseResourceManger.hpp"
#include "AnimatedCharacter.hpp"

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

private:
    void ValidTask();

private:
    enum class Phase {
        Welcome,
        PICK_STAGE,
        STAGE_ONE,
        // STAGE_TWO,
        // STAGE_THREE,
        // STAGE_FOUR,
        //CHANGE_CHARACTER_IMAGE,
        //ABLE_TO_MOVE,
        //COLLIDE_DETECTION,
        BEE_ANIMATION,
        OPEN_THE_DOORS,
        COUNTDOWN,
    };


    State m_CurrentState = State::START;
    Phase m_Phase = Phase::Welcome;

    Util::Renderer m_Root;

    std::shared_ptr<Character> m_pico1;
    std::shared_ptr<Character> m_pico2;

    std::shared_ptr<Character> m_Giraffe;
    std::shared_ptr<Character> m_Chest;
    std::vector<std::shared_ptr<Character>> m_Doors;

    std::shared_ptr<AnimatedCharacter> m_Bee;
    std::shared_ptr<AnimatedCharacter> m_Ball;

    std::shared_ptr<PhaseResourceManger> m_PRM;

    bool m_EnterDown = false;
};

#endif
