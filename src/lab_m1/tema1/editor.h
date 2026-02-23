#pragma once

#include "lab_m1/tema1/tema_transform2D.h"
#include "lab_m1/tema1/render.h"

using namespace m2;

namespace m1
{
    class editor
    {
    public:
        editor();
        ~editor();

        void Init(render* rendererPtr);
        void Update(float deltaTimeSeconds);

        void LeftClick(float x, float y);
        void MouseDrag(float x, float y);
        bool LeftRelease(float x, float y);
        bool RightClick(float x, float y);

        bool Row();
        bool Palette() const;
        bool StartGame() const;

        void Array(bool copy[ROWS][COLS]);
        void Reset();

        bool GetCell(int rows, int cols) const;
        int GetCount() const;

        bool inStart(float x, float y);


        static const int rows = 9;
        static const int cols = 17;

    private:
        render* renderer;

        bool drag;
        glm::vec2 dragPos;
        glm::vec2 offset;

        bool first;
        int initial; 
        int count;
        bool valid;

    };
}