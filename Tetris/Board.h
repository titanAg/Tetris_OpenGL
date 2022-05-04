#pragma once
#include "Block.h"
#include "Tetris.h"
#include "Tetromino.h";
class Board
{
private:
	vector<vector<Block>> board;
	vector<vector<Block>> ghostBoard;
	vector<Tetromino> pieces;
	Tetromino storedPiece;
	Tetromino tempStoredPiece;
	Tetromino lastPiece; 
	int t_minI = 20, t_maxI = -1, t_minJ = 10, t_maxJ = -1;
	int gt_minI = 20, gt_maxI = -1, gt_minJ = 10, gt_maxJ = -1;
	bool isGameOver = true;
	bool hasStoredBeenUsed = false;
	int rowsRemoved = 0;
	int score = 0;
	int previousLevel = 1;

public:
	Board();
	void setGameOverBool(bool gameOver);
	int getGameOverBool();
	void initializeBoard(int option);
	void displayBoard1();
	void displayBoard2();
	void displayPieces();
	void displayStoredPiece();
	void resetBoardState(int option);
	Tetromino getNextPiece();
	void swap(vector<vector<Block>>& board, int i1, int j1, int i2, int j2);
	bool getMoveDownCollisionBool(int option);
	bool getMoveDownCollisionBool(int i, int j);
	bool getSideMoveCollisionBool(int shift);
	void shiftGhostBlocksDown();
	void shiftBlocksDown();
	void hardShiftBlocksDown();
	void removeTetromino();
	void storePiece();
	void shiftBlocksUp(int row);
	void addDeadBlockRow();
	void shiftBlocksLeft();
	void shiftBlocksRight();
	void updateScore(int removedRowCount);
	void checkForFullRows();
	void removeRow(int i);
	void updateFullRowState(int i);
	vector<int> getPivotPoint(int t_id);
	void rotateTetromino();
	void rotateLeft(int t_id, int minI, int maxI, int minJ, int maxJ);
	void rotateRight(int t_id, int minI, int maxI, int minJ, int maxJ);
	bool getRotationCollisionBool(int minI, int maxI, int minJ, int maxJ);
	bool testValueAndState(int i, int j, int value, int state);
	void updateMinMax();
	void addTetromino(int shift);
	void createGhostTetromino();
	int getBlockValue(int i, int j);
	int getBlockState(int i, int j);
	int getGhostValue(int i, int j);
	Tetromino getPiece(int index);
	Tetromino getStoredPiece();
	int getNextPieceWidth(int index);
	int getMinI();
	int getMaxI();
	int getMinJ();
	int getMaxJ();
	int getTetrominoId();
	int getRandomInt();
	int getRowsRemoved();
	int getScore();
	int getLevel();
	void addToScore(int points);
};


