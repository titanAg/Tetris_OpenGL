#pragma once
#include "Tetris.h"
class Tetromino
{
private:
	vector<vector<Block>> tetromino;
	vector<int> minIndices;
	int t_id;
	int shift;
public:

	void displayTetromino();

	Tetromino(int randomInt);

	Tetromino();

	void initializeTetromino();

	vector<int> getMinMaxIndices();

	vector<vector<Block>> getTetromino();

	int getShift();

	int getRandomShiftInt(int mod);

	int getValue(int i, int j);

	int getTetrominoWidth();

	int getTetrominoHeight();

	int getTetrominoId();

	void randomizePlacement();

	void generateTetromino(int selection);

	void findMinMaxIndices();

	void createTetromino(int randomInt);

	void rotateTetromino();

	void rotateLeft(int minI, int maxI, int minJ, int maxJ);

	void rotateRight(int minI, int maxI, int minJ, int maxJ);

};

