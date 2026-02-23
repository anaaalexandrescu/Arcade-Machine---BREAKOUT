#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema1/render.h"
#include "lab_m1/tema1/game.h"
#include "lab_m1/tema1/editor.h" 

namespace m1
{
    class tema1 : public gfxc::SimpleScene
    {
    public:
        tema1();
        ~tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;

        void StartGame();
        bool Start(float x, float y);

    protected:
        static const int rows = 5;
        static const int cols = 12;

        static constexpr float screenW = 1280.0f;
        static constexpr float screenH = 720.0f;

        enum class SceneType
        {
            EDITOR,
            GAME
        };

        render renderer;
        game* gamePtr;
        editor* editorPtr; 

        SceneType currentScene;

        bool lastFrame;
        bool rightLastFrame;
    };
}