#include "lab_m1/tema1/tema1.h"
#include "lab_m1/tema1/tema_transform2D.h"
#include <iostream>

using namespace std;
using namespace m1;
using namespace m2;

tema1::tema1()
{
}

tema1::~tema1()
{
}

void tema1::Init()
{
    renderer.Init();

    lastFrame = false;
    rightLastFrame = false;

    currentScene = SceneType::EDITOR;

    editorPtr = new editor();
    editorPtr->Init(&renderer);

    gamePtr = new game();
}

void tema1::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void tema1::StartGame()
{
    bool ship[ROWS][COLS];
    editorPtr->Array(ship);

    if (gamePtr) {
        gamePtr->Init(ship, &renderer);
    }

    currentScene = SceneType::GAME;
}

void tema1::Update(float deltaTimeSeconds)
{
    if (currentScene == SceneType::EDITOR) {
        if (editorPtr) {
            editorPtr->Update(deltaTimeSeconds);
        }
    }
    else if (currentScene == SceneType::GAME) {
        if (gamePtr) {
            gamePtr->Update(deltaTimeSeconds);
        }
    }
}

void tema1::OnInputUpdate(float deltaTime, int mods)
{
    bool mouse = window->MouseHold(GLFW_MOUSE_BUTTON_LEFT);
    bool mouseRight = window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT);
    glm::ivec2 resolution = window->GetResolution(); 
    glm::vec2 mousePos = window->GetCursorPosition(); 

    float x, y;

    x = mousePos.x / resolution.x * screenW;
    y = (resolution.y - mousePos.y) / resolution.y * screenH; 

    if (currentScene == SceneType::GAME) {
        lastFrame = mouse;
        rightLastFrame = mouseRight;
        return;
    }

    if (mouseRight && editorPtr && rightLastFrame == false) {
        editorPtr->RightClick(x, y);
    }

    if (mouse && lastFrame == false) {
        if (Start(x, y)) {
            if (editorPtr && editorPtr->StartGame()) {
                StartGame();
            }
        }
        else {
            if (editorPtr) {
                editorPtr->LeftClick(x, y);
            }
        }
    }

    if (mouse && editorPtr) {
        editorPtr->MouseDrag(x, y);
    }

    if (mouse == false && editorPtr && lastFrame) {
        editorPtr->LeftRelease(x, y);
    }

    lastFrame = mouse;
    rightLastFrame = mouseRight;
}

bool tema1::Start(float x, float y)
{
    float buttonX = 1220.0f;
    float buttonY = 635.0f;
    float buttonW = 50.0f;
    float buttonH = 50.0f;

    bool start_game = x >= buttonX && x <= buttonX + buttonW && y >= buttonY && y <= buttonY + buttonH;

    return start_game;
}

void tema1::FrameEnd() 
{
}

void tema1::OnKeyPress(int key, int mods)
{
    if (currentScene == SceneType::GAME && gamePtr) {
        gamePtr->OnKeyPress(key);
    }
}

void tema1::OnKeyRelease(int key, int mods)
{
    if (currentScene == SceneType::GAME && gamePtr) {
        if (key == GLFW_KEY_LEFT) {
            gamePtr->left = false;
        }
        if (key == GLFW_KEY_RIGHT) {
            gamePtr->right = false;
        }
    }
}