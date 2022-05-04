// Kyle Orcutt
// Obj for individual piece state and operations
#include "Tetromino.h"


/*DEBUG - displays tetromino block values in console*/
void Tetromino::displayTetromino() {
	for (int i = 0; i < tetromino.size(); i++) {
		for (int j = 0; j < tetromino[i].size(); j++) {
			cout << tetromino[i][j].getValue() << " ";
		}
		cout << endl;
	}
	cout << "\n";
}

Tetromino::Tetromino() {
	vector<vector<Block>> tetromino(4, vector<Block>(4, Block()));
	this->tetromino = tetromino;
}

Tetromino::Tetromino(int randomInt) {
	vector<vector<Block>> tetromino(4, vector<Block>(4, Block()));
	this->tetromino = tetromino;
	createTetromino(randomInt);
}

/*Sets all blocks in a tetromino to a value & state of 0. */
void Tetromino::initializeTetromino() {
	for (int i = 0; i < tetromino.size(); i++) {
		for (int j = 0; j < tetromino[i].size(); j++) {
			tetromino[i][j] = Block(i, j, 0, 0);
		}
	}
}

vector<int> Tetromino::getMinMaxIndices() {
	return minIndices;
}

vector<vector<Block>> Tetromino::getTetromino() {
	return tetromino;
}

int Tetromino::getShift() {
	return shift;
}

int Tetromino::getRandomShiftInt(int mod) {
	return rand() % mod;
}

int Tetromino::getValue(int i, int j) {
	return tetromino[i][j].getValue();
}

//void Tetromino::setValue(int id) {
//	generateTetromino(id);
//}

int Tetromino::getTetrominoWidth() {
	return minIndices[3] - minIndices[1] + 1;
}

int Tetromino::getTetrominoHeight() {
	return minIndices[2] - minIndices[0] + 1;
}

int Tetromino::getTetrominoId() {
	for (int i = 0; i < tetromino.size(); i++) {
		for (int j = 0; j < tetromino[i].size(); j++) {
			if (tetromino[i][j].getValue() > 0)
				return tetromino[i][j].getValue();
		}
	}
	return 0;
}

void Tetromino::randomizePlacement() {
	// width 1 = shift 0-9   width 2 = shift 0-8   width 3 = shift 0-7   width 4 = shift 0-6
	// indices 1-7 = letters O-T

	int width = getTetrominoWidth();
	switch (width) {
	case 1:
		shift = getRandomShiftInt(10);
		break;
	case 2:
		shift = getRandomShiftInt(9);
		break;
	case 3:
		shift = getRandomShiftInt(8);
		break;
	default:
		shift = getRandomShiftInt(7);
	}
}

/*Initializes tetromino and assigns values to the appropriate
  indices to form the selected tetromino shape.
  Parameters: selection # = # of cooresponding tetromino shape */
void Tetromino::generateTetromino(int selection) {
	initializeTetromino();
	switch (selection) {
	case 1:
		tetromino[1][1] = Block(1, 1, 1, 1);
		tetromino[1][2] = Block(1, 2, 1, 1);
		tetromino[2][1] = Block(2, 1, 1, 1);
		tetromino[2][2] = Block(2, 2, 1, 1);
		break;
	case 2:
		tetromino[1][0] = Block(1, 1, 2, 1);
		tetromino[1][1] = Block(1, 2, 2, 1);
		tetromino[1][2] = Block(2, 1, 2, 1);
		tetromino[1][3] = Block(2, 2, 2, 1);
		break;
	case 3:
		tetromino[1][2] = Block(1, 1, 3, 1);
		tetromino[1][3] = Block(1, 2, 3, 1);
		tetromino[2][1] = Block(2, 1, 3, 1);
		tetromino[2][2] = Block(2, 2, 3, 1);
		break;
	case 4:
		tetromino[1][1] = Block(1, 1, 4, 1);
		tetromino[1][2] = Block(1, 2, 4, 1);
		tetromino[2][2] = Block(2, 1, 4, 1);
		tetromino[2][3] = Block(2, 2, 4, 1);
		break;
	case 5:
		tetromino[1][1] = Block(1, 1, 5, 1);
		tetromino[1][2] = Block(1, 2, 5, 1);
		tetromino[1][3] = Block(2, 1, 5, 1);
		tetromino[2][1] = Block(2, 2, 5, 1);
		break;
	case 6:
		tetromino[1][1] = Block(1, 1, 6, 1);
		tetromino[1][2] = Block(1, 2, 6, 1);
		tetromino[1][3] = Block(2, 1, 6, 1);
		tetromino[2][3] = Block(2, 2, 6, 1);
		break;
	case 7:
		tetromino[1][1] = Block(1, 1, 7, 1);
		tetromino[1][2] = Block(1, 2, 7, 1);
		tetromino[1][3] = Block(2, 1, 7, 1);
		tetromino[2][2] = Block(2, 2, 7, 1);
		break;
	}

	rotateTetromino();
	randomizePlacement();
}

/*Find min i & j index.
  Use: determine index shift when tetromino is merged into main board */
void Tetromino::findMinMaxIndices() {
	//minIndices = { min i , min j , max i , max j }
	minIndices = { 3,3,0,0 };
	for (int i = 0; i < tetromino.size() - 1; i++) {
		for (int j = 0; j < tetromino[i].size(); j++) {
			if (tetromino[i][j].getValue() > 0) {
				if (i < minIndices[0])
					minIndices[0] = i;
				if (j < minIndices[1])
					minIndices[1] = j;
				if (i > minIndices[2])
					minIndices[2] = i;
				if (j > minIndices[3])
					minIndices[3] = j;
			}
		}
	}
}

/*Generates a number to select a random tetromino shape.
  Then creates another number to pick the shape's location.
  Then calls functions to add shape to board and create the ghost tetromino */
void Tetromino::createTetromino(int randomInt) {
	t_id = randomInt;
	generateTetromino(randomInt);
}

void Tetromino::rotateTetromino()
{
	//srand(time(NULL));
	int rotationCount = rand() % 4 + 1;
	findMinMaxIndices();
	int minI = 0, maxI = 3, minJ = 0, maxJ = 3;
	int iSpan = minIndices[2] - minIndices[0];
	int jSpan = minIndices[3] - minIndices[1];

	minJ++;
	maxI--;
	for (int i = 0; i < rotationCount; i++) {
		if (t_id >= 2 && t_id <= 4) {
			if (iSpan > jSpan)
				rotateRight(minI, maxI, minJ, maxJ);
			else
				rotateLeft(minI, maxI, minJ, maxJ);
		}
		else if (t_id >= 5) {
			rotateLeft(minI, maxI, minJ, maxJ);
		}
	}
	findMinMaxIndices();
}

void Tetromino::rotateLeft(int minI, int maxI, int minJ, int maxJ) {
	if (t_id == 2) {
		Block temp = tetromino[minI + 1][minJ - 1];
		tetromino[minI + 1][minJ - 1] = tetromino[maxI + 1][maxJ - 1];
		tetromino[maxI + 1][maxJ - 1] = temp;
	}

	Block temp = tetromino[minI][minJ];
	tetromino[minI][minJ] = tetromino[minI][maxJ];
	tetromino[minI][maxJ] = tetromino[maxI][maxJ];
	tetromino[maxI][maxJ] = tetromino[maxI][minJ];
	tetromino[maxI][minJ] = temp;

	temp = tetromino[minI][minJ + 1];
	tetromino[minI][minJ + 1] = tetromino[minI + 1][maxJ];
	tetromino[minI + 1][maxJ] = tetromino[maxI][minJ + 1];
	tetromino[maxI][minJ + 1] = tetromino[minI + 1][minJ];
	tetromino[minI + 1][minJ] = temp;
}

void Tetromino::rotateRight(int minI, int maxI, int minJ, int maxJ) {
	if (t_id == 2) {
		Block temp = tetromino[minI + 1][minJ - 1];
		tetromino[minI + 1][minJ - 1] = tetromino[maxI + 1][maxJ - 1];
		tetromino[maxI + 1][maxJ - 1] = temp;
	}
	Block temp = tetromino[minI][minJ];
	tetromino[minI][minJ] = tetromino[maxI][minJ];
	tetromino[maxI][minJ] = tetromino[maxI][maxJ];
	tetromino[maxI][maxJ] = tetromino[minI][maxJ];
	tetromino[minI][maxJ] = temp;

	temp = tetromino[minI + 1][minJ];
	tetromino[minI + 1][minJ] = tetromino[maxI][minJ + 1];
	tetromino[maxI][minJ + 1] = tetromino[minI + 1][maxJ];
	tetromino[minI + 1][maxJ] = tetromino[minI][minJ + 1];
	tetromino[minI][minJ + 1] = temp;
}
