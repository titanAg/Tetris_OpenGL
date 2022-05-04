// Kyle Orcutt
// Main game class
// Responsible for all the high level logic/game mechanics along with the OpenGL aspect of the project

#include "Tetris.h"
#include "Board.h"
#define GLEW_STATIC 

const GLint WIDTH = 850;
const GLint HEIGHT = 850;
const GLfloat shrinkMultiplier = 0.9;
int score = 0;
double gameSpeed = 1;
int previousLevel = 1;
bool isHardShiftPressed = false;
bool isGamePaused = false;

GLFWwindow* window = NULL;
Board b;

GLuint lines_vao, mainBlock_vao, ghostBlock_vao, nb_vao, nbl_vao, sbl_vao, sb_vao;
GLuint mainBlock_vbo, ghostBlock_vbo, n_vbo, nbl_vbo, sbl_vbo, sb_vbo;
GLuint vboLines;
GLuint shaderProgram;

int blockVectorSize, ghostVectorSize, nextBlockVectorSize, nextBlockLinesVectorSize, storedBlockVectorSize, storedBlockLinesVectorSize;

const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec3 color;"
"\n"
"out vec3 vert_color;"
"void main()"
"{"
"    vert_color = color;"
"    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"in vec3 vert_color;"
"out vec4 frag_color;"
"void main()"
"{"
"    frag_color = vec4(vert_color, 1.0f);"
"}";

void showDebugValues() {
	//cout << "Rows Removed: " << b.getRowsRemoved() << "\n";
	//cout << "Game speed: " << gameSpeed << "\n";
	//b.displayBoard1();
	//b.displayStoredPiece();
	//showLevelAndScore();
	/*b.displayPieces();
	cout << "\n\n";*/
}

int setupGLWindow() {
	//initialize GLFW
	if (!glfwInit()) {
		cout << "GLFW init failed";
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//core mean no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//window = glfwCreateWindow(1800, 1800, "Tetris", glfwGetPrimaryMonitor(), NULL);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Tetris", NULL, NULL);
	glfwSetWindowPos(window, 550, 100);
	if (!window) {
		cout << "GLFW window creation failed!";
		glfwTerminate();
		return 1;
	}

	//Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	cout << "buffer width " << bufferWidth << endl;

	//set context use
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW init failed";
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	//setup viewport size - size of what we are drawing to
	glViewport(0, 0, bufferWidth, bufferHeight);
}

vector<GLfloat> getColor(int value) {
	vector<GLfloat> color;
	switch (value) {
		//yellow
	case 1:
		color = { 0.9, 0.9, 0.0 };
		break;
		//aqua
	case 2:
		color = { 0.0, 1.0, 1.0 };
		break;
		//green
	case 3:
		color = { 0.0, 1.0, 0.0 };
		break;
		//red
	case 4:
		color = { 1.0, 0.0, 0.0 };
		break;
		//orange
	case 5:
		color = { 1.0, 0.5, 0.0 };
		break;
		//blue
	case 6:
		color = { 0.0, 0.33, 1.0 };
		break;
		//purple
	case 7:
		color = { 0.5, 0.0, 1.0 };
		break;
		//black
	case 8:
		color = { 0.3, 0.3, 0.3 };
		break;
		//white
	case 9:
		color = { 1.0, 1.0, 1.0 };
		break;
	}
	return color;
}

vector<GLfloat> getLightColor(int value) {
	vector<GLfloat> color;
	switch (value) {
		//yellow
	case 1:
		color = { 1.0, 1.0, 0.9 };
		break;
		//aqua
	case 2:
		color = { 0.8, 1.0, 1.0 };
		break;
		//green
	case 3:
		color = { 0.8, 1.0, 0.8 };
		break;
		//red
	case 4:
		color = { 1.0, 0.7, 0.7 };
		break;
		//orange
	case 5:
		color = { 1.0, 0.85, 0.7 };
		break;
		//blue
	case 6:
		color = { 0.7, 0.8, 1.0 };
		break;
		//purple
	case 7:
		color = { 0.85, 0.7, 1.0 };
		break;
		//black
	case 8:
		color = { 0.5, 0.5, 0.5 };
		break;
		//white
	case 9:
		color = { 1.0, 1.0, 1.0 };
		break;
	}
	return color;
}

vector<GLfloat> getDarkColor(int value) {
	vector<GLfloat> color;
	switch (value) {
		//yellow
	case 1:
		color = { 0.5, 0.5, 0.0 };
		break;
		//aqua
	case 2:
		color = { 0.0, 0.5, 0.5 };
		break;
		//green
	case 3:
		color = { 0.0, 0.3, 0.0 };
		break;
		//red
	case 4:
		color = { 0.2, 0.0, 0.0 };
		break;
		//orange
	case 5:
		color = { 0.5, 0.2, 0.0 };
		break;
		//blue
	case 6:
		color = { 0.0, 0.1, 0.3 };
		break;
		//purple
	case 7:
		color = { 0.2, 0.0, 0.45 };
		break;
		//black
	case 8:
		color = { 0.1, 0.1, 0.1 };
		break;
		//white
	case 9:
		color = { 0.8, 0.8, 0.8 };
		break;
	}
	return color;
}

void addBlockVerticesToVector(vector<GLfloat>& vertices, GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, int id, int state) {
	GLfloat centerX = (x2 - x1) / 2 + x1;
	GLfloat centerY = (y2 - y1) / 2 + y1;
	GLfloat innerX1 = x1 + ((x2 - x1) * 0.2);
	GLfloat innerY1 = y1 + ((y2 - y1) * 0.2);
	GLfloat innerX2 = x2 - ((x2 - x1) * 0.2);
	GLfloat innerY2 = y2 - ((y2 - y1) * 0.2);

	if (centerX == 0.0 && centerY == 0.0 || innerX1 == 0.0 && innerY1 == 0.0 || innerX2 == 0.0 && innerY2 == 0.0) {
		cout << "center: " << centerX << "," << centerY << endl;
		cout << "inner1: " << innerX1 << "," << innerY1 << endl;
		cout << "inner2: " << innerX2 << "," << innerX2 << endl;
	}

	vector<GLfloat> color = getColor(id);
	vector<GLfloat> blackOffset = getColor(8);
	vector<GLfloat> whiteOffset = getColor(9);

	vector<GLfloat> lightColor = getLightColor(id);
	vector<GLfloat> darkColor = getDarkColor(id);

	if (state == 3)
		color = getColor(9);
	//add outer square (4 triangles)
	//top left
	vertices.push_back(x1);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(lightColor[0]);
	vertices.push_back(lightColor[1]);
	vertices.push_back(lightColor[2]);
	//top right
	vertices.push_back(x2);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(lightColor[0]);
	vertices.push_back(lightColor[1]);
	vertices.push_back(lightColor[2]);
	//center
	vertices.push_back(centerX);
	vertices.push_back(centerY);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//top right
	vertices.push_back(x2);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(darkColor[0]);
	vertices.push_back(darkColor[1]);
	vertices.push_back(darkColor[2]);
	//bottom right
	vertices.push_back(x2);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);
	//center
	vertices.push_back(centerX);
	vertices.push_back(centerY);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//bottom right
	vertices.push_back(x2);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(darkColor[0]);
	vertices.push_back(darkColor[1]);
	vertices.push_back(darkColor[2]);
	//bottom left
	vertices.push_back(x1);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(darkColor[0]);
	vertices.push_back(darkColor[1]);
	vertices.push_back(darkColor[2]);
	//center
	vertices.push_back(centerX);
	vertices.push_back(centerY);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//bottom left
	vertices.push_back(x1);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);
	//top left
	vertices.push_back(x1);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(darkColor[0]);
	vertices.push_back(darkColor[1]);
	vertices.push_back(darkColor[2]);
	//center
	vertices.push_back(centerX);
	vertices.push_back(centerY);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);


	//add inner square vertices
	//top left
	vertices.push_back(innerX1);
	vertices.push_back(innerY1);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);
	//bottom right
	vertices.push_back(innerX2);
	vertices.push_back(innerY2);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);
	//bottom left
	vertices.push_back(innerX1);
	vertices.push_back(innerY2);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//top left
	vertices.push_back(innerX1);
	vertices.push_back(innerY1);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);
	//top right
	vertices.push_back(innerX2);
	vertices.push_back(innerY1);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);
	//bottom right
	vertices.push_back(innerX2);
	vertices.push_back(innerY2);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

}

void addGhostBlocksToVector(vector<GLfloat>& vertices, GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, int id) {
	vector<GLfloat> color = getColor(id);

	//top left
	vertices.push_back(x1);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//top right
	vertices.push_back(x2);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//top right
	vertices.push_back(x2);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//bottom right
	vertices.push_back(x2);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//bottom right
	vertices.push_back(x2);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//bottom left
	vertices.push_back(x1);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//bottom left
	vertices.push_back(x1);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);

	//top left
	vertices.push_back(x1);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);
}

void addSidePieceBordersToVector(vector<GLfloat>& vertices, GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, int i, int j) {

	GLfloat gridColors[2][3] = { { 0.0f, 0.0f, 0.0f }, { 0.3f, 0.3f, 0.3f } };

	//load outer border 
	if (i == 0) {
		//downShift = y1;
		//top left
		vertices.push_back(x1);
		vertices.push_back(y1);
		vertices.push_back(0.0f);
		vertices.push_back(gridColors[1][0]);
		vertices.push_back(gridColors[1][1]);
		vertices.push_back(gridColors[1][2]);
		//top right
		vertices.push_back(x2);
		vertices.push_back(y1);
		vertices.push_back(0.0f);
		vertices.push_back(gridColors[1][0]);
		vertices.push_back(gridColors[1][1]);
		vertices.push_back(gridColors[1][2]);
	}
	if (i == 3) {
		//downShift = y2;
		//bottom right
		vertices.push_back(x2);
		vertices.push_back(y2);
		vertices.push_back(0.0f);
		vertices.push_back(gridColors[1][0]);
		vertices.push_back(gridColors[1][1]);
		vertices.push_back(gridColors[1][2]);
		//bottom left
		vertices.push_back(x1);
		vertices.push_back(y2);
		vertices.push_back(0.0f);
		vertices.push_back(gridColors[1][0]);
		vertices.push_back(gridColors[1][1]);
		vertices.push_back(gridColors[1][2]);
	}
	if (j == 0) {
		//rightShift = x1;
		//top left
		vertices.push_back(x1);
		vertices.push_back(y1);
		vertices.push_back(0.0f);
		vertices.push_back(gridColors[1][0]);
		vertices.push_back(gridColors[1][1]);
		vertices.push_back(gridColors[1][2]);
		//bottom left
		vertices.push_back(x1);
		vertices.push_back(y2);
		vertices.push_back(0.0f);
		vertices.push_back(gridColors[1][0]);
		vertices.push_back(gridColors[1][1]);
		vertices.push_back(gridColors[1][2]);
	}
	if (j == 3) {
		//rightShift = x2;
		//top right
		vertices.push_back(x2);
		vertices.push_back(y1);
		vertices.push_back(0.0f);
		vertices.push_back(gridColors[1][0]);
		vertices.push_back(gridColors[1][1]);
		vertices.push_back(gridColors[1][2]);
		//bottom right
		vertices.push_back(x2);
		vertices.push_back(y2);
		vertices.push_back(0.0f);
		vertices.push_back(gridColors[1][0]);
		vertices.push_back(gridColors[1][1]);
		vertices.push_back(gridColors[1][2]);
	}
}

void addSidePieceInnerBordersToVector(vector<GLfloat>& vertices, GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2) {

	GLfloat gridColors[2][3] = { { 0.0f, 0.0f, 0.0f }, { 0.3f, 0.3f, 0.3f } };

	//top left
	vertices.push_back(x1);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(gridColors[0][0]);
	vertices.push_back(gridColors[0][1]);
	vertices.push_back(gridColors[0][2]);

	//bottom left
	vertices.push_back(x1);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(gridColors[0][0]);
	vertices.push_back(gridColors[0][1]);
	vertices.push_back(gridColors[0][2]);

	//bottom left
	vertices.push_back(x1);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(gridColors[0][0]);
	vertices.push_back(gridColors[0][1]);
	vertices.push_back(gridColors[0][2]);

	//bottom right
	vertices.push_back(x2);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(gridColors[0][0]);
	vertices.push_back(gridColors[0][1]);
	vertices.push_back(gridColors[0][2]);

	//bottom right
	vertices.push_back(x2);
	vertices.push_back(y2);
	vertices.push_back(0.0f);
	vertices.push_back(gridColors[0][0]);
	vertices.push_back(gridColors[0][1]);
	vertices.push_back(gridColors[0][2]);

	//top right
	vertices.push_back(x2);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(gridColors[0][0]);
	vertices.push_back(gridColors[0][1]);
	vertices.push_back(gridColors[0][2]);

	//top right
	vertices.push_back(x2);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(gridColors[0][0]);
	vertices.push_back(gridColors[0][1]);
	vertices.push_back(gridColors[0][2]);

	//top left
	vertices.push_back(x1);
	vertices.push_back(y1);
	vertices.push_back(0.0f);
	vertices.push_back(gridColors[0][0]);
	vertices.push_back(gridColors[0][1]);
	vertices.push_back(gridColors[0][2]);
}

void addNextPiecesTo_nb_vao() {
	vector<GLfloat> nextBlockPoints;
	vector<GLfloat> nextBlockLines;
	for (int index = 0; index < 3; index++) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Tetromino t = b.getPiece(index);
				int id = t.getValue(i, j);
				GLfloat gridColors[2][3] = { { 0.0f, 0.0f, 0.0f }, { 0.3f, 0.3f, 0.3f } };
				vector<int> indices = t.getMinMaxIndices();
				int width = t.getTetrominoWidth();
				int height = t.getTetrominoHeight();
				GLfloat rightShift = 0.0;
				GLfloat downShift = 0.0;

				GLfloat x1 = (((GLfloat)j / 5 - 1) / 2);
				GLfloat y1 = 1 - ((GLfloat)i / 10);
				GLfloat x2 = x1 + 0.1;
				GLfloat y2 = y1 - 0.1;
				x1 += 1.1;
				x2 += 1.1;

				GLfloat downOffset = 0;

				if (index == 0) {
					downOffset = 0.0;
				}
				else if (index == 1) {
					downOffset = 0.5;
				}
				else if (index == 2) {
					downOffset = 1.0;
				}

				y1 -= downOffset;
				y2 -= downOffset;


				x1 *= shrinkMultiplier;
				x2 *= shrinkMultiplier;
				y1 *= shrinkMultiplier;
				y2 *= shrinkMultiplier;

				//load outer border 
				addSidePieceBordersToVector(nextBlockLines, x1, x2, y1, y2, i, j);

				if (width == 1 || width == 3) {
					x1 -= 0.05 * shrinkMultiplier;
					x2 -= 0.05 * shrinkMultiplier;
				}
				else if (width == 2 && indices[1] == 2) {
					x1 -= 0.1 * shrinkMultiplier;
					x2 -= 0.1 * shrinkMultiplier;
				}

				if (height == 1 || (height == 3 && id != 2)) {
					y1 -= 0.05 * shrinkMultiplier;
					y2 -= 0.05 * shrinkMultiplier;
				}
				else if (height == 2 && indices[2] == 1) {
					y1 -= 0.1 * shrinkMultiplier;
					y2 -= 0.1 * shrinkMultiplier;
				}

				//load blocks
				if (id > 0) {
					//blocks
					addBlockVerticesToVector(nextBlockPoints, x1, x2, y1, y2, t.getTetrominoId(), b.getBlockState(i, j));

					//block borders 
					addSidePieceInnerBordersToVector(nextBlockLines, x1, x2, y1, y2);
				}

			}
		}
	}
	glGenBuffers(1, &n_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, n_vbo);
	glBufferData(GL_ARRAY_BUFFER, nextBlockPoints.size() * sizeof(GLfloat), nextBlockPoints.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &nb_vao);
	glBindVertexArray(nb_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(0); //attribute 0 - the location
	glEnableVertexAttribArray(1); //attribute 1 - the color

	glGenBuffers(1, &nbl_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbl_vbo);
	glBufferData(GL_ARRAY_BUFFER, nextBlockLines.size() * sizeof(GLfloat), nextBlockLines.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &nbl_vao);
	glBindVertexArray(nbl_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(0); //attribute 0 - the location
	glEnableVertexAttribArray(1); //attribute 1 - the color

	nextBlockVectorSize = nextBlockPoints.size();
	nextBlockPoints.clear();

	nextBlockLinesVectorSize = nextBlockLines.size();
	nextBlockLines.clear();
}

void addStoredPieceTo_nb_vao() {
	vector<GLfloat> storedBlockPoints;
	vector<GLfloat> storedBlockLines;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Tetromino t = b.getStoredPiece();
			int id = t.getValue(i, j);
			GLfloat gridColors[2][3] = { { 0.0f, 0.0f, 0.0f }, { 0.3f, 0.3f, 0.3f } };
			vector<int> indices = t.getMinMaxIndices();
			int width, height;
			if (t.getTetrominoId() > 0) {
				width = t.getTetrominoWidth();
				height = t.getTetrominoHeight();
			}
			GLfloat rightShift = 0.0;
			GLfloat downShift = 0.0;

			GLfloat x1 = (((GLfloat)j / 5 - 1) / 2);
			GLfloat y1 = 1 - ((GLfloat)i / 10);
			GLfloat x2 = x1 + 0.1;
			GLfloat y2 = y1 - 0.1;
			x1 -= 0.5;
			x2 -= 0.5;

			x1 *= shrinkMultiplier;
			x2 *= shrinkMultiplier;
			y1 *= shrinkMultiplier;
			y2 *= shrinkMultiplier;

			//load outer border 
			addSidePieceBordersToVector(storedBlockLines, x1, x2, y1, y2, i, j);

			if (t.getTetrominoId() > 0) {
				if (width == 1 || width == 3) {
					x1 -= 0.05 * shrinkMultiplier;
					x2 -= 0.05 * shrinkMultiplier;
				}
				else if (width == 2 && indices[1] == 2) {
					x1 -= 0.1 * shrinkMultiplier;
					x2 -= 0.1 * shrinkMultiplier;
				}

				if (height == 1 || (height == 3 && id != 2)) {
					y1 -= 0.05 * shrinkMultiplier;
					y2 -= 0.05 * shrinkMultiplier;
				}
				else if (height == 2 && indices[2] == 1) {
					y1 -= 0.1 * shrinkMultiplier;
					y2 -= 0.1 * shrinkMultiplier;
				}
			}

			//load blocks
			if (id > 0) {
				//blocks
				addBlockVerticesToVector(storedBlockPoints, x1, x2, y1, y2, t.getTetrominoId(), b.getBlockState(i, j));

				//block borders 
				addSidePieceInnerBordersToVector(storedBlockLines, x1, x2, y1, y2);
			}

		}
	}
	glGenBuffers(1, &sb_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sb_vbo);
	glBufferData(GL_ARRAY_BUFFER, storedBlockPoints.size() * sizeof(GLfloat), storedBlockPoints.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &sb_vao);
	glBindVertexArray(sb_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(0); //attribute 0 - the location
	glEnableVertexAttribArray(1); //attribute 1 - the color

	glGenBuffers(1, &sbl_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sbl_vbo);
	glBufferData(GL_ARRAY_BUFFER, storedBlockLines.size() * sizeof(GLfloat), storedBlockLines.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &sbl_vao);
	glBindVertexArray(sbl_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(0); //attribute 0 - the location
	glEnableVertexAttribArray(1); //attribute 1 - the color

	storedBlockVectorSize = storedBlockPoints.size();
	storedBlockPoints.clear();

	storedBlockLinesVectorSize = storedBlockLines.size();
	storedBlockLines.clear();
}

void addBoardBlocksTo_vao() {
	vector<GLfloat> blockPoints;
	vector<GLfloat> ghostBlockPoints;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			GLfloat x1 = (((GLfloat)j / 5 - 1) / 2) * shrinkMultiplier;
			GLfloat y1 = (1 - (GLfloat)i / 10) * shrinkMultiplier;
			GLfloat x2 = x1 + (GLfloat)0.1 * shrinkMultiplier;
			GLfloat y2 = y1 - (GLfloat)0.1 * shrinkMultiplier;


			if (b.getBlockValue(i, j) > 0) {
				//blocks
				addBlockVerticesToVector(blockPoints, x1, x2, y1, y2, b.getBlockValue(i, j), b.getBlockState(i, j));
			}

			if (b.getGhostValue(i, j) > 0 && b.getBlockValue(i, j) == 0) {
				//ghost blocks
				addGhostBlocksToVector(ghostBlockPoints, x1, x2, y1, y2, b.getGhostValue(i, j));

			}
		}
	}
	glGenBuffers(1, &mainBlock_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mainBlock_vbo);
	glBufferData(GL_ARRAY_BUFFER, blockPoints.size() * sizeof(GLfloat), blockPoints.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &mainBlock_vao);
	glBindVertexArray(mainBlock_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(0); //attribute 0 - the location
	glEnableVertexAttribArray(1); //attribute 1 - the color

	glGenBuffers(1, &ghostBlock_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ghostBlock_vbo);
	glBufferData(GL_ARRAY_BUFFER, ghostBlockPoints.size() * sizeof(GLfloat), ghostBlockPoints.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &ghostBlock_vao);
	glBindVertexArray(ghostBlock_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(0); //attribute 0 - the location
	glEnableVertexAttribArray(1); //attribute 1 - the color

	blockVectorSize = blockPoints.size();
	ghostVectorSize = ghostBlockPoints.size();
	blockPoints.clear();
	ghostBlockPoints.clear();
}

void addGridTo_vaoLines() {
	GLfloat gridColors[3] = { 0.3f, 0.3f, 0.3f };
	GLfloat rowPoints[504];
	for (int i = 0; i < 37; i++) {
		//add horizontal lines
		if (i < 21) {
			rowPoints[i * 12] = 0.5f * shrinkMultiplier;//x0
			rowPoints[i * 12 + 1] = (0.1f * i - 1) * shrinkMultiplier;//y0
			rowPoints[i * 12 + 2] = 0.0f;
			rowPoints[i * 12 + 3] = gridColors[0];
			rowPoints[i * 12 + 4] = gridColors[1];
			rowPoints[i * 12 + 5] = gridColors[2];

			rowPoints[i * 12 + 6] = -0.5f * shrinkMultiplier;//x1
			rowPoints[i * 12 + 7] = (0.1f * i - 1) * shrinkMultiplier;//y1
			rowPoints[i * 12 + 8] = 0.0f;
			rowPoints[i * 12 + 9] = gridColors[0];
			rowPoints[i * 12 + 10] = gridColors[1];
			rowPoints[i * 12 + 11] = gridColors[2];
		}
		//add vertical lines
		else if (i > 25) {
			rowPoints[i * 12] = (0.1f * (i - 21) - 1) * shrinkMultiplier;//x0
			rowPoints[i * 12 + 1] = shrinkMultiplier;//y0	
			rowPoints[i * 12 + 2] = 0.0f;

			rowPoints[i * 12 + 3] = gridColors[0];
			rowPoints[i * 12 + 4] = gridColors[1];
			rowPoints[i * 12 + 5] = gridColors[2];

			rowPoints[i * 12 + 6] = (0.1f * (i - 21) - 1) * shrinkMultiplier;//x1
			rowPoints[i * 12 + 7] = -shrinkMultiplier;//y1
			rowPoints[i * 12 + 8] = 0.0f;

			rowPoints[i * 12 + 9] = gridColors[0];
			rowPoints[i * 12 + 10] = gridColors[1];
			rowPoints[i * 12 + 11] = gridColors[2];
		}
	}

	//grid lines buffer
	glGenVertexArrays(1, &lines_vao);
	glBindVertexArray(lines_vao);

	glGenBuffers(1, &vboLines);
	glBindBuffer(GL_ARRAY_BUFFER, vboLines);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rowPoints), rowPoints, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(0); //attribute 0 - the location
	glEnableVertexAttribArray(1); //attribute 1 - the color
}

//void loadBlockVAO() {
//	glGenBuffers(1, &block_vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, block_vbo);
//	glBufferData(GL_ARRAY_BUFFER, block_vertices.size() * sizeof(GLfloat), block_vertices.data(), GL_STATIC_DRAW);
//
//	glGenVertexArrays(1, &block_vao);
//	glBindVertexArray(block_vao);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
//	glEnableVertexAttribArray(0); //attribute 0 - the location
//	glEnableVertexAttribArray(1); //attribute 1 - the color
//}
//
//void loadLineVAO() {
//	glGenBuffers(1, &line_vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, line_vbo);
//	glBufferData(GL_ARRAY_BUFFER, line_vertices.size() * sizeof(GLfloat), line_vertices.data(), GL_STATIC_DRAW);
//
//	glGenVertexArrays(1, &line_vao);
//	glBindVertexArray(line_vao);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
//	glEnableVertexAttribArray(0); //attribute 0 - the location
//	glEnableVertexAttribArray(1); //attribute 1 - the color
//}

void showLevelAndScore() {
	ostringstream outs;
	outs.precision(3);	// decimal places
	outs << fixed
		<< "Tetris" << "    "
		<< "Level: " << b.getLevel() << "    "
		<< "Score: " << b.getScore() << "    ";
	glfwSetWindowTitle(window, outs.str().c_str());
}

void updateGameConditions(int level) {
	gameSpeed = 1 - ((double)level / 10) * 0.7;
	b.addDeadBlockRow();
}

void initializeGame() {
	srand(time(NULL));
	b = Board();
	b.setGameOverBool(false);
	b.initializeBoard(1);
	b.initializeBoard(2);
	b.addTetromino(1);
	showLevelAndScore();
	previousLevel = 1;
	isHardShiftPressed = false;
	isGamePaused = false;
}

void updateGame() {
	static double previousSeconds = 0.0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime();
	elapsedSeconds = currentSeconds - previousSeconds;

	if (elapsedSeconds > gameSpeed && !isGamePaused) {
		previousSeconds = currentSeconds;

		if (!b.getGameOverBool()) {

			if (b.getMoveDownCollisionBool(1) || isHardShiftPressed) {
				b.resetBoardState(1);
				b.checkForFullRows();
				b.addTetromino(1);
				if (previousLevel != b.getLevel()) {
					previousLevel = b.getLevel();
					updateGameConditions(b.getLevel());
				}
				isHardShiftPressed = false;
			}
			else {
				b.shiftBlocksDown();
			}

		}

		showDebugValues();
	}
	if (elapsedSeconds > 0.20) {
		showLevelAndScore();
	}
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		initializeGame();
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		isGamePaused = !isGamePaused;
	}

	else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT) && b.getMinJ() > 0 && !b.getSideMoveCollisionBool(-1) && blockVectorSize > 0 && !b.getGameOverBool() && !isGamePaused) {
		b.shiftBlocksLeft();
	}
	else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT) && b.getMaxJ() < 9 && !b.getSideMoveCollisionBool(1) && blockVectorSize > 0 && !b.getGameOverBool() && !isGamePaused) {
		b.shiftBlocksRight();
	}

	else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT) && blockVectorSize > 0 && !b.getGameOverBool() && !b.getMoveDownCollisionBool(1) && !isGamePaused) {
		b.shiftBlocksDown();
		b.addToScore(1);
	}

	else if (key == GLFW_KEY_UP && action == GLFW_PRESS && blockVectorSize > 0 && !b.getGameOverBool() && !isGamePaused) {
		b.rotateTetromino();
	}

	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && blockVectorSize > 0 && !b.getGameOverBool() && !isGamePaused) {
		b.hardShiftBlocksDown();
		isHardShiftPressed = true;
	}

	else if (key == GLFW_KEY_C && action == GLFW_PRESS && blockVectorSize > 0 && !b.getGameOverBool() && !isGamePaused) {
		b.storePiece();
	}

}

int main()
{
	setupGLWindow();
	addGridTo_vaoLines();
	glfwSetKeyCallback(window, glfw_onKey);

	//createShader();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);
	glCompileShader(vs);

	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		cout << "Error - Vertex shader failed to compile. " << infoLog << endl;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
		cout << "Error - Fragment shader failed to compile. " << infoLog << endl;
	}

	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		cout << "Error - Shader program linker failed " << infoLog << endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);


	initializeGame();
	while (!glfwWindowShouldClose(window)) {
		addBoardBlocksTo_vao();
		addNextPiecesTo_nb_vao();
		addStoredPieceTo_nb_vao();
		updateGame();

		//get and handle user input events
		glfwWaitEventsTimeout(0.1);
		glfwPollEvents();

		//clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(mainBlock_vao);
		glDrawArrays(GL_TRIANGLES, 0, blockVectorSize);

		glBindVertexArray(nb_vao);
		glDrawArrays(GL_TRIANGLES, 0, nextBlockVectorSize);

		glBindVertexArray(sb_vao);
		glDrawArrays(GL_TRIANGLES, 0, storedBlockVectorSize);

		glBindVertexArray(nbl_vao);
		glDrawArrays(GL_LINES, 0, nextBlockLinesVectorSize);

		glBindVertexArray(sbl_vao);
		glDrawArrays(GL_LINES, 0, storedBlockLinesVectorSize);

		glBindVertexArray(lines_vao);
		glDrawArrays(GL_LINES, 0, 252);

		glBindVertexArray(ghostBlock_vao);
		glDrawArrays(GL_LINES, 0, ghostVectorSize);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &lines_vao);
	glDeleteVertexArrays(1, &mainBlock_vao);
	glDeleteVertexArrays(1, &ghostBlock_vao);
	glDeleteVertexArrays(1, &nb_vao);
	glDeleteVertexArrays(1, &nbl_vao);
	glDeleteVertexArrays(1, &sbl_vao);
	glDeleteVertexArrays(1, &sb_vao);
	glDeleteBuffers(1, &vboLines);
	glDeleteBuffers(1, &mainBlock_vbo);
	glDeleteBuffers(1, &ghostBlock_vbo);
	glDeleteBuffers(1, &n_vbo);
	glDeleteBuffers(1, &nbl_vbo);
	glDeleteBuffers(1, &sbl_vbo);
	glDeleteBuffers(1, &sb_vbo);
	glfwTerminate();


	return 0;
}