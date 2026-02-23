#include "lab_m1/tema1/editor.h"
#include <iostream>

using namespace m1;

editor::editor()
{
}

editor::~editor()
{
}

void editor::Init(render* rendererPtr)
{
    renderer = rendererPtr;

    initial = -1;
    count = 0;

    drag = false;
    first = false;
    valid = true;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            renderer->layout[i][j] = false;
            renderer->type[i][j] = 0;
        }
    }

    renderer->count = 10;
}

void editor::Update(float deltaTimeSeconds)
{
    valid = Row();

    renderer->Grid();
    renderer->GreenSquares();
    renderer->BlueSquares();
    renderer->GreySquare();
    renderer->SetSquares(drag, dragPos, renderer->layout);

    if (StartGame() == false) {
        renderer->StopButton();
    }
    else {
        renderer->StartButton();
    }
}

bool editor::Row()
{
    int total = 0;
    int minCol = cols;
    int maxCol = -1;
    int currentRow = -1;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (renderer->layout[i][j]) {
                total++;

                if (currentRow != -1 && i != currentRow) {
                    count = total;
                    return false;
                }

                if (currentRow == -1) {
                    currentRow = i;
                }

                if (j < minCol) 
                    minCol = j;

                if (j > maxCol) 
                    maxCol = j;
            }
        }
    }
    count = total;

    if (total > 10) {
        return false;
    }

    if (total <= 1) {
        return true;
    }

    for (int i = minCol; i <= maxCol; i++) {
        if (renderer->layout[currentRow][i] != true) {
            return false;
        }
    }

    return true;
}

bool editor::StartGame() const
{
    bool ok = valid && (renderer->count > 0 || count == 10);

    return ok;
}

void editor::LeftClick(float x, float y)
{
    float minX = POSITION.x;
    float minY = POSITION.y;
    float maxX = minX + SIDE;
    float maxY = minY + SIDE;

    if (x >= minX && x <= maxX) {
        if (y >= minY && y <= maxY) {
            drag = true;
            offset.x = minX - x;
            offset.y = minY - y;
            dragPos.x = x + offset.x;
            dragPos.y = y + offset.y;
        }
        
    }
    else
    {
        drag = false;
    }
}

void editor::MouseDrag(float x, float y)
{
    if (drag) {
        dragPos.x = x + offset.x;
        dragPos.y = y + offset.y;
    }
}

bool editor::LeftRelease(float x, float y)
{
    if (drag == false) {
        return drag;
    }

    drag = false;

    glm::vec2 center = dragPos + glm::vec2(SIDE * 0.5f);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            glm::vec2 cellCenter = renderer->GetCenter(i, j);
            float halfSide = renderer->GetSide() * 0.5f;

                if (center.x < cellCenter.x - halfSide)
                continue; 

                if (center.x > cellCenter.x + halfSide)
                continue; 

                if (center.y < cellCenter.y - halfSide)
                continue; 

                if (center.y > cellCenter.y + halfSide)
                continue;
            
                if (renderer->layout[i][j])
                {
                    return true;
                }

                renderer->layout[i][j] = true;
                renderer->type[i][j] = 1;

                if (first == false) {
                    initial = i;
                    first = true;
                }

                if (renderer->count > 0) {
                    renderer->count--;
                }

                return true;
        }
    }

    return false;
}

bool editor::RightClick(float x, float y)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (renderer->layout[i][j]) {
                glm::vec2 center = renderer->GetCenter(i, j);

                if ((x - center.x) * (x - center.x) <= (SIDE * 0.5f) * (SIDE * 0.5f) &&
                    (y - center.y) * (y - center.y) <= (SIDE * 0.5f) * (SIDE * 0.5f))
                {
                    renderer->layout[i][j] = false;
                    renderer->type[i][j] = 0;

                    bool empty = true;
                    for (int k = 0; k < cols; k++) {
                        if (renderer->layout[i][k]) {
                            empty = false;
                            break;
                        }
                    }

                    if (empty && i == initial) {
                        first = false;
                        initial = -99;
                    }

                    int total = 0;
                    for (int i = 0; i < rows; i++) {
                        for (int j = 0; j < cols; j++) {
                            if (renderer->layout[i][j]) {
                                total++;
                            }
                        }
                    }

                    if (total < 10 && renderer->count < 10) {
                        renderer->count++;
                    }

                    return true;
                }
            }
        }
    }

    return false;
}

bool editor::inStart(float x, float y)
{
    float xX = 1220.0f;
    float yY = 635.0f;
    float width = 50.0f;
    float height = 50.0f;

    bool ok = x >= xX && x <= xX + width && y >= yY && y <= yY + height;

    return ok;
}

void editor::Array(bool copy[ROWS][COLS])
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            copy[i][j] = renderer->layout[i][j];
        }
    }
}

void editor::Reset()
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            renderer->layout[i][j] = false;
            renderer->type[i][j] = 0;
        }
    }
    renderer->count = 10;
    initial = -1;
    count = 0;
    first = false;
}

bool editor::GetCell(int row, int col) const
{
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return true;
    }
    return false;
}

bool editor::Palette() const
{ 
    return valid; 
}

int editor::GetCount() const 
{
    return count; 
}