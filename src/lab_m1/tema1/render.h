#pragma once
#include "components/simple_scene.h"
#include <vector>

#define ROWS 9
#define COLS 17
#define SIDE 55.0f
#define POSITION glm::vec2(115.0f, 75.0f)

namespace m2
{
    class render : public gfxc::SimpleScene
    {
    public:
        render();
        ~render();
        void Init() override;

        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void GreenSquares();
        void BlueSquares();
        void Grid();
        void GreySquare();
        void StartButton();
        void StopButton();
        void SetSquares(bool drag, glm::vec2 dragPos, bool okSquare[ROWS][COLS]);
        void Rectangle(float x, float y, float width, float height);
        void Circle(float x, float y, float radius);

        void Rectangle(float x, float y, float width, float height, int color);

        float GetSide();
        glm::vec2 GetCenter(int row, int col);

    public:
        glm::mat3 modelMatrix;
        float cx, cy;

        int count;
        float blueSide;
        glm::vec2 centers[ROWS][COLS];
        bool layout[ROWS][COLS];
        int type[ROWS][COLS];

        void Text(float x, float y, float size, const char* text);
        void Digit(float x, float y, float size, int digit);
        void Char(float x, float y, float size, char chr);

        void Score(int score);
        void Lives(int lives);
    };
}