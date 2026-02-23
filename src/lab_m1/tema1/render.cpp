#include <vector>
#include <iostream>
#include "render.h"

using namespace std;
using namespace m2;

#include "lab_m1/tema1/tema_transform2D.h"
#include "lab_m1/tema1/tema_object2D.h"

float x, y;

render::render()
{
}

render::~render()
{
}

void render::Init() 
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);


	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	cx = corner.x + squareSide / 2;
	cy = corner.y + squareSide / 2;

	AddMeshToList(object2D::CreateSquare("square1", corner, squareSide, glm::vec3(0, 1, 1), true));
	AddMeshToList(object2D::CreateSquare("blue", corner, 1, glm::vec3(0.3f, 0.5f, 1.0f), true));
	AddMeshToList(object2D::CreateSquare("green", corner, 50, glm::vec3(0.0f, 0.9f, 0.0f), true));

	AddMeshToList(object2D::CreateSquare("brick_0", corner, 1, glm::vec3(0.0f, 0.0f, 1.0f), true));
	AddMeshToList(object2D::CreateSquare("brick_1", corner, 1, glm::vec3(0.0f, 1.0f, 0.0f), true));
	AddMeshToList(object2D::CreateSquare("brick_2", corner, 1, glm::vec3(1.0f, 0.0f, 0.0f), true));
	AddMeshToList(object2D::CreateSquare("brick_3", corner, 1, glm::vec3(1.0f, 0.6f, 0.0f), true));
	AddMeshToList(object2D::CreateSquare("brick_4", corner, 1, glm::vec3(0.6f, 0.0f, 1.0f), true));
	AddMeshToList(object2D::CreateSquare("brick_default", corner, 1, glm::vec3(0.75f, 0.75f, 0.75f), true));

	AddMeshToList(object2D::CreateSquare("paddle_rect", corner, 1, glm::vec3(1.0f, 1.0f, 1.0f), true));

	AddMeshToList(object2D::CreateSquare("red_outline", glm::vec3(0, 0, 0), 1, glm::vec3(1, 0, 0)));
	AddMeshToList(object2D::CreateSquare("blue_outline", glm::vec3(0, 0, 0), 1, glm::vec3(0, 0, 1)));


	{
		float color = 0.1f;

		glm::vec3 corner = glm::vec3(0.6f, 0.6f, 0.6f);
		glm::vec3 center = glm::vec3(0.95f, 0.95f, 0.95f);

		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0, 0, color), corner),
			VertexFormat(glm::vec3(1, 0, color), corner),
			VertexFormat(glm::vec3(1, 1, color), corner),
			VertexFormat(glm::vec3(0, 1, color), corner),
			VertexFormat(glm::vec3(0.5f, 0.5f, color), center)
		};

		std::vector<unsigned int> indices = {
			0, 1, 4,
			2, 3, 4,
			1, 2, 4,
			3, 0, 4 
		};

		CreateMesh("square", vertices, indices);
	}


	{
		glm::vec3 color = glm::vec3(1.0f, 0.4f, 0.7f);
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;

		int seg = 32;
		vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));

		for (int i = 0; i <= seg; i++) {
			float angle = 2.0f * 3.14f * i / seg;
			float x = cos(angle);
			float y = sin(angle);
			vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
		}

		for (int i = 1; i <= seg; i++) {
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i + 1);
		}

		CreateMesh("circle", vertices, indices);
	}


	{
		glm::vec3 color = glm::vec3(0.0f, 0.9f, 0.0f);

		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0, 0, 0), color),
			VertexFormat(glm::vec3(1, 0, 0), color),
			VertexFormat(glm::vec3(1, 1, 0), color),
			VertexFormat(glm::vec3(0, 1, 0), color),
		};

		std::vector<unsigned int> indices = {
			0, 2, 3,
			0, 1, 3 
		};

		CreateMesh("green_start", vertices, indices);
	}

	{
		glm::vec3 color = glm::vec3(1.0f, 0.1f, 0.1f);

		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0, 0, 0), color),
			VertexFormat(glm::vec3(1, 0, 0), color),
			VertexFormat(glm::vec3(1, 1, 0), color),
			VertexFormat(glm::vec3(0, 1, 0), color),
		};

		std::vector<unsigned int> indices = {
			0, 2, 3,
			0, 1, 3
		};

		CreateMesh("red_start", vertices, indices);
	}

	float x = 315.0f;
	float y = 25.0f;

	float width = 920.0f;
	float height = 520.0f;

	float segX = (float)(COLS * 2 + 1);
	float segY = (float)(ROWS * 2 + 1);

	float tempX = width / segX;
	float tempY = height / segY;

	float size;

	if (tempX < tempY) {
		size = tempX;
	}
	else {
		size = tempY;
	}

	float side = size;

	blueSide = side + 10;
	count = 10;


	for (int j = 0; j < ROWS; ++j) {
		for (int i = 0; i < COLS; ++i) {
			float corner_x = x + size + (float)i * 2.0f * size;
			float corner_y = y + size + (float)j * 2.0f * size;

			centers[j][i] = glm::vec2(corner_x + blueSide * 0.5f, corner_y + blueSide * 0.5f);

			layout[j][i] = false;
			type[j][i] = 0;
		}
	}
}

float render::GetSide()
{
	return blueSide;
}

glm::vec2 render::GetCenter(int row, int col)
{
	if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
		return centers[row][col];
	}
	else return glm::vec2(-99, -99);
}

void render::Grid()
{
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(5, 5);
	modelMatrix *= transform2D::Scale(1270, 710);

	glLineWidth(0.01f);
	RenderMesh2D(meshes["red_outline"], shaders["VertexColor"], modelMatrix);

	x = 5;
	y = 5;

	for (int i = 0; i < 4; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(x, y + i * 180);
		modelMatrix *= transform2D::Scale(260, 180);

		RenderMesh2D(meshes["red_outline"], shaders["VertexColor"], modelMatrix);
	}
}

void render::GreenSquares()
{
	float x = 280;
	float y = 635;

	for (int i = 0; i < count; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(x + i * 94, y);
		RenderMesh2D(meshes["green"], shaders["VertexColor"], modelMatrix);
	}
}

void render::BlueSquares()
{
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(315, 25);
	modelMatrix *= transform2D::Scale(920, 520);

	RenderMesh2D(meshes["blue_outline"], shaders["VertexColor"], modelMatrix);


	for (int j = 0; j < ROWS; j++) {
		for (int i = 0; i < COLS; i++) {
			glm::vec2 center = centers[j][i];
			glm::vec2 corner = center - glm::vec2(blueSide * 0.5f);


			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(corner.x, corner.y);
			modelMatrix *= transform2D::Scale(blueSide, blueSide);
			RenderMesh2D(meshes["blue"], shaders["VertexColor"], modelMatrix);
		}
	}
}

void render::GreySquare()
{
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(POSITION.x, POSITION.y);
	modelMatrix *= transform2D::Scale(SIDE, SIDE);

	RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
}

void render::StartButton()
{
	float x = 1220, y = 635;
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(x, y);
	modelMatrix *= transform2D::Scale(50, 50);

	RenderMesh2D(meshes["green_start"], shaders["VertexColor"], modelMatrix);
}

void render::StopButton()
{
	float startX = 1220, startY = 635;
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(startX, startY);
	modelMatrix *= transform2D::Scale(50, 50);

	RenderMesh2D(meshes["red_start"], shaders["VertexColor"], modelMatrix);
}

void render::SetSquares(bool drag, glm::vec2 dragPos, bool okSquare[ROWS][COLS])
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (okSquare[i][j]) {
				glm::vec2 center = this->centers[i][j];
				glm::vec2 corner = center - glm::vec2(SIDE * 0.5f);

				modelMatrix = glm::mat3(1);
				modelMatrix *= transform2D::Translate(corner.x, corner.y);
				modelMatrix *= transform2D::Scale(SIDE, SIDE);

				RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
			}
		}
	}

	if (drag) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(dragPos.x, dragPos.y);
		modelMatrix *= transform2D::Scale(SIDE, SIDE);
		RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
	}
}

void render::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices) 
{

	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO_ID;

	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	GLuint IBO_ID;

	glGenBuffers(1, &IBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	glBindVertexArray(0);

	if (GetOpenGLError() == GL_INVALID_OPERATION) {

		cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;

		cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;

	}

	meshes[name] = new Mesh(name);

	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));

}

void render::Rectangle(float x, float y, float width, float height)
{
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(x, y);
	modelMatrix *= transform2D::Scale(width, height);
	RenderMesh2D(meshes["paddle_rect"], shaders["VertexColor"], modelMatrix);
}

void render::Rectangle(float x, float y, float width, float height, int colorType)
{
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(x, y);
	modelMatrix *= transform2D::Scale(width, height);

	std::string meshName;
	if (colorType == 5) {
		meshName = "brick_4";
	}
	else if (colorType == 4) {
		meshName = "brick_3";
	}
	else if (colorType == 3) {
		meshName = "brick_2";
	}
	else if (colorType == 2) {
		meshName = "brick_0";
	}
	else if (colorType == 1) {
		meshName = "brick_1";
	}
	else {
		meshName = "brick_default";
	}

	RenderMesh2D(meshes[meshName], shaders["VertexColor"], modelMatrix);
}

void render::Circle(float x, float y, float radius)
{
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(x, y);
	modelMatrix *= transform2D::Scale(radius, radius);
	RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
}

void render::Char(float x, float y, float size, char chr)
{
	bool map[5][3] = { {0} };

	if (chr == '0') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][0] = map[1][2] = 1;
		map[2][0] = map[2][2] = 1;
		map[3][0] = map[3][2] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == '1') {
		map[0][1] = map[1][1] = map[2][1] = map[3][1] = map[4][1] = 1;
	}
	else if (chr == '2') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][2] = 1;
		map[2][0] = map[2][1] = map[2][2] = 1;
		map[3][0] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == '3') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][2] = 1;
		map[2][0] = map[2][1] = map[2][2] = 1;
		map[3][2] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == '4') {
		map[0][0] = map[0][2] = 1;
		map[1][0] = map[1][2] = 1;
		map[2][0] = map[2][1] = map[2][2] = 1;
		map[3][2] = map[4][2] = 1;
	}
	else if (chr == '5') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][0] = 1;
		map[2][0] = map[2][1] = map[2][2] = 1;
		map[3][2] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == '6') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][0] = 1;
		map[2][0] = map[2][1] = map[2][2] = 1;
		map[3][0] = map[3][2] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == '7') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][2] = map[2][2] = map[3][2] = map[4][2] = 1;
	}
	else if (chr == '8') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][0] = map[1][2] = 1;
		map[2][0] = map[2][1] = map[2][2] = 1;
		map[3][0] = map[3][2] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == '9') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][0] = map[1][2] = 1;
		map[2][0] = map[2][1] = map[2][2] = 1;
		map[3][2] = map[4][2] = 1;
	}
	else if (chr == 'S') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][0] = 1;
		map[2][0] = map[2][1] = map[2][2] = 1;
		map[3][2] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == 'c') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][0] = map[2][0] = map[3][0] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == 'o') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][0] = map[1][2] = 1;
		map[2][0] = map[2][2] = 1;
		map[3][0] = map[3][2] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == 'r') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][0] = map[1][2] = 1;
		map[2][0] = map[2][1] = 1;
		map[3][0] = map[3][2] = 1;
		map[4][0] = map[4][2] = 1;
	}
	else if (chr == 'e') {
		map[0][0] = map[0][1] = map[0][2] = 1;
		map[1][0] = 1;
		map[2][0] = map[2][1] = map[2][2] = 1;
		map[3][0] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == 'L') {
		map[0][0] = map[1][0] = map[2][0] = map[3][0] = 1;
		map[4][0] = map[4][1] = map[4][2] = 1;
	}
	else if (chr == 'i') {
		map[0][1] = 1;
		map[2][1] = map[3][1] = map[4][1] = 1;
	}
	else if (chr == 'v') {
		map[0][0] = map[0][2] = 1;
		map[1][0] = map[1][2] = 1;
		map[2][0] = map[2][2] = 1;
		map[3][0] = map[3][2] = 1;
		map[4][1] = 1;
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (map[i][j])
			{
				float xX = x + j * size;
				float yY = y - i * size;
				Rectangle(xX, yY, size, size);
			}
		}
	}
}

void render::Text(float x, float y, float size, const char* text)
{
	float currentX = x;
	float charWidth = size * 4;

	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] == ' ') {
			currentX += charWidth / 2;
		}
		else {
			Char(currentX, y, size, text[i]);
			currentX += charWidth;
		}
	}
}

void render::Digit(float x, float y, float size, int digit)
{
	if (digit < 0 || digit > 9)
		return;
	char c = '0' + digit;
	Char(x, y, size, c);
}

void render::Score(int score)
{
	float x = 50.0f;
	float y = 700.0f;
	float size = 5.0f;
	float space = size * 4;

	Text(x, y, size, "Score ");

	float numX = x + 6 * space;

	int d1 = score % 10;
	int d2 = (score / 10) % 10;

	Digit(numX + 2 * space, y, size, d1);

	if (score >= 10) {
		Digit(numX + 1 * space, y, size, d2);
	}
}

void render::Lives(int lives)
{
	float x = 1100.0f;
	float y = 700.0f;
	float size = 5.0f;
	float space = size * 4;

	Text(x, y, size, "LiveS    ");

	float numX = x + 7 * space;

	if (lives < 0) lives = 0;
	Digit(numX, y, size, lives % 10);
}