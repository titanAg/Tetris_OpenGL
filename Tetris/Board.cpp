// Kyle Orcutt
// Main game grid and side pieces
// Represents overall board state and provides operations to the game class
#include "Board.h"

/*DEBUG - displays board block values in console*/
void Board::displayBoard1() {
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			cout << board[i][j].getValue() << " ";
		}
		cout << endl;
	}
	cout << "\n";
}

/*DEBUG - displays board block states in console*/
void Board::displayBoard2() {
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			cout << board[i][j].getState() << " ";
		}
		cout << endl;
	}
	cout << "\n";
}

void Board::displayPieces() {
	for (int i = 1; i < pieces.size(); i++) {
		pieces[i].displayTetromino();
		cout << "\n\n";
	}

}

void Board::displayStoredPiece() {
	storedPiece.displayTetromino();
}

Board::Board() {
	vector<vector<Block>> board(20, vector<Block>(10, Block()));
	this->board = board;
	vector<vector<Block>> ghostBoard(20, vector<Block>(10, Block()));
	this->ghostBoard = ghostBoard;
	this->pieces = { Tetromino(getRandomInt()), Tetromino(getRandomInt()), Tetromino(getRandomInt()), Tetromino(getRandomInt()) };
	this->storedPiece = Tetromino();
	this->tempStoredPiece = Tetromino();
}

void Board::setGameOverBool(bool gameOver) {
	isGameOver = gameOver;
}

int Board::getGameOverBool() {
	return isGameOver;
}

int Board::getBlockValue(int i, int j) {
	return board[i][j].getValue();
}

int Board::getBlockState(int i, int j) {
	return board[i][j].getState();
}

int Board::getGhostValue(int i, int j) {
	return ghostBoard[i][j].getValue();
}

Tetromino Board::getPiece(int index) {
	return pieces[index];
}

Tetromino Board::getStoredPiece() {
	return storedPiece;
}

int Board::getNextPieceWidth(int index) {
	return pieces[index].getTetrominoWidth();
}

int Board::getMinI() {
	return t_minI;
}

int Board::getMaxI() {
	return t_maxI;
}

int Board::getMinJ() {
	return t_minJ;
}

int Board::getMaxJ() {
	return t_maxJ;
}

int Board::getTetrominoId() {
	for (int i = t_minI; i <= t_maxI; i++) {
		for (int j = t_minJ; j <= t_maxJ; j++) {
			if (board[i][j].getValue() > 0)
				return board[i][j].getValue();
		}
	}
}

int Board::getRandomInt() {
	return rand() % 7 + 1;
}

int Board::getRowsRemoved() {
	return rowsRemoved;
}

int Board::getScore() {
	return score;
}

int Board::getLevel() {
	if (rowsRemoved >= 270)
		return 10;
	else if (rowsRemoved >= 220)
		return 9;
	else if (rowsRemoved >= 175)
		return 8;
	else if (rowsRemoved >= 135)
		return 7;
	else if (rowsRemoved >= 100)
		return 6;
	else if (rowsRemoved >= 70)
		return 5;
	else if (rowsRemoved >= 45)
		return 4;
	else if (rowsRemoved >= 25)
		return 3;
	else if (rowsRemoved >= 10)
		return 2;

		return 1;
}

void Board::addToScore(int points) {
	score += points * getLevel();
}

/*Sets all blocks in a board to a value & state of 0.
  Use: board setup and reset
  Parameters: option 1 = main board, option 2 = ghost board */
void Board::initializeBoard(int option) {
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			if (option == 1)
				board[i][j] = Block(i, j, 0, 0);
			else if (option == 2)
				ghostBoard[i][j] = Block(i, j, 0, 0);
		}
	}
}

/*Sets all board blocks to a state of 0.
  Use: freeze moving blocks
  Parameters: option 1 = main board, option 2 = ghost board */
void Board::resetBoardState(int option) {
	int minI = 0, maxI = 0, minJ = 0, maxJ = 0;
	if (option == 1) {
		minI = t_minI, maxI = t_maxI, minJ = t_minJ, maxJ = t_maxJ;
	}
	else if (option == 2) {
		minI = gt_minI, maxI = gt_maxI, minJ = gt_minJ, maxJ = gt_maxJ;
	}
	for (int i = minI; i <= maxI; i++) {
		for (int j = minJ; j <= maxJ; j++) {
			if (option == 1)
				board[i][j].setState(0);
			if (option == 2)
				ghostBoard[i][j].setState(0);
		}
	}
}

Tetromino Board::getNextPiece() {
	Tetromino t = pieces[0];
	pieces[0] = pieces[1];
	pieces[1] = pieces[2];
	pieces[2] = pieces[3];
	pieces[3] = Tetromino(getRandomInt());
	return t;
}

/*Add tetromino to main board and update min/max i & j index values.
  Parameter: shift = difference from tetromino index to board index */
void Board::addTetromino(int option) {
	Tetromino t = NULL;
	if (option == 1) {
		t = getNextPiece();
		tempStoredPiece = t;		
		hasStoredBeenUsed = false;
	}
	else if (option == 2) {
		t = storedPiece;
	}
	int shift = t.getShift();

	vector<vector<Block>> tetromino = t.getTetromino();
	vector<int> minIndices = t.getMinMaxIndices();

	t_minI = 20, t_maxI = -1, t_minJ = 10, t_maxJ = -1;
	gt_minI = 20, gt_maxI = -1, gt_minJ = 10, gt_maxJ = -1;

	for (int i = 0; i < 4; i++) {
		for (int j = shift; j < tetromino.size() + shift; j++) {
			if (tetromino[i][j - shift].getValue() > 0 && board[i - minIndices[0]][j - minIndices[1]].getValue() == 0) {
				board[i - minIndices[0]][j - minIndices[1]] = tetromino[i][j - shift];
				if (i - minIndices[0] < t_minI) {
					t_minI = i - minIndices[0];
				}
				if (i - minIndices[0] > t_maxI) {
					t_maxI = i - minIndices[0];
				}
				if (j - minIndices[1] < t_minJ) {
					t_minJ = j - minIndices[1];
				}
				if (j - minIndices[1] > t_maxJ) {
					t_maxJ = j - minIndices[1];
				}
			}
			else if (tetromino[i][j - shift].getValue() > 0 && board[i - minIndices[0]][j - minIndices[1]].getValue() > 0) {
				isGameOver = true;
				resetBoardState(1);
				return;
			}
		}
	}
	createGhostTetromino();
	minIndices.clear();
}

/*Creates a ghost tetromino on the ghost board and calls a function to move the ghost to the bottom.
  Note - The ghost tetromino is used to track where the active tetromino will sit on the bottom. */
void Board::createGhostTetromino() {
	int lowerBound = t_minI, upperBound = t_maxI, leftBound = t_minJ, rightBound = t_maxJ;
	initializeBoard(2);

	for (int i = lowerBound; i <= upperBound; i++) {
		for (int j = leftBound; j <= rightBound; j++) {
			if (board[i][j].getState() == 1) {
				ghostBoard[i][j].setState(1);
				ghostBoard[i][j].setValue(board[i][j].getValue());
				gt_minI = t_minI, gt_maxI = t_maxI, gt_minJ = t_minJ, gt_maxJ = t_maxJ;
			}
		}
	}
	shiftGhostBlocksDown();
}

/*Swaps block state and value of i1/j1 -> i2/j2 in the board passed in. */
void Board::swap(vector<vector<Block>>& board, int i1, int j1, int i2, int j2) {
	board[i2][j2].setValue(board[i1][j1].getValue());
	board[i2][j2].setState(board[i1][j1].getState());
	board[i1][j1].setValue(0);
	board[i1][j1].setState(0);
}

/*Returns true if a shift down will cause a collision between the selected
  tetromino and another tetromino or the bottom of the main board.
  Parameters: option 1 = active tetromino, option 2 = ghost tetromino */
bool Board::getMoveDownCollisionBool(int option) {
	int minI = 0, maxI = 0, minJ = 0, maxJ = 0;
	if (option == 1)
		minI = t_minI, maxI = t_maxI, minJ = t_minJ, maxJ = t_maxJ;
	else if (option == 2)
		minI = gt_minI, maxI = gt_maxI, minJ = gt_minJ, maxJ = gt_maxJ;

	if (maxI == 19)
		return true;
	for (int i = minI; i <= maxI; i++) {
		for (int j = minJ; j <= maxJ; j++) {
			bool isMovableState = (option == 1 ? board[i][j].getState() == 1 : ghostBoard[i][j].getState() == 1);
			if (i + 1 < board.size() && isMovableState && board[i + 1][j].getValue() > 0 && board[i + 1][j].getState() == 0)
				return true;
		}
	}
	return false;
}

/*Returns true if a shift down from i & j will cause a collision between the
  selected block and another tetromino or the bottom of the main board.*/
bool Board::getMoveDownCollisionBool(int i, int j) {
	if (i == 19)
		return true;
	if (i + 1 < board.size() && board[i + 1][j].getValue() > 0 && board[i + 1][j].getState() == 0)
		return true;
	return false;
}

/*Moves ghost tetromino to the lowest possible location on the board.*/
void Board::shiftGhostBlocksDown() {
	int lowerBound = t_minI, upperBound = t_maxI, leftBound = t_minJ, rightBound = t_maxJ;

	if (getMoveDownCollisionBool(2)) {
		resetBoardState(2);
		return;
	}

	for (int k = 0; k < board.size(); k++) {
		bool isFirstShift = true;
		for (int i = board.size() - 1; i >= t_minI; i--) {
			for (int j = t_maxJ; j >= t_minJ; j--) {
				if (i < board.size() - 1 && ghostBoard[i][j].getState() == 1 && ghostBoard[i + 1][j].getValue() == 0) {
					swap(ghostBoard, i, j, i + 1, j);
					if (isFirstShift) {
						gt_minI++;
						gt_maxI++;
						isFirstShift = false;
					}
				}
			}
		}
		if (getMoveDownCollisionBool(2)) {
			resetBoardState(2);
			return;
		}
	}
}

/*Moves active tetromino down by one on the board.*/
void Board::shiftBlocksDown() {
	bool isFirstShift = true;
	int lowerBound = t_minI, upperBound = t_maxI, leftBound = t_minJ, rightBound = t_maxJ;

	for (int i = upperBound; i >= lowerBound; i--) {
		for (int j = rightBound; j >= leftBound; j--) {
			if (i < board.size() - 1 && board[i][j].getState() == 1 && board[i + 1][j].getValue() == 0) {
				swap(board, i, j, i + 1, j);
				if (isFirstShift) {
					t_minI++;
					t_maxI++;
					isFirstShift = false;
				}
			}
		}
	}
}

void Board::hardShiftBlocksDown() {
	for (int i = gt_minI; i <= gt_maxI; i++) {
		for (int j = gt_minJ; j <= gt_maxJ; j++) {
			if (ghostBoard[i][j].getValue() > 0) {
				board[i][j].setValue(ghostBoard[i][j].getValue());
				board[i][j].setState(0);
			}
		}
	}
	removeTetromino();
	addToScore((gt_minI - t_minI) * 2);
	t_minI = gt_minI, t_maxI = gt_maxI, t_minJ = gt_minJ, t_maxJ = gt_maxJ;
}

/*Returns true if a shift left/right will cause a collision between the
  active block and another tetromino or the side of the main board.
  Parameters: shift -1 = left, shift 1 = right */
bool Board::getSideMoveCollisionBool(int shift) {
	for (int i = t_minI; i <= t_maxI; i++) {
		for (int j = t_minJ; j <= t_maxJ; j++) {
			if (j + shift < 10 && board[i][j].getState() == 1 && board[i][j + shift].getValue() > 0 && board[i][j + shift].getState() == 0)
				return true;
		}
	}
	return false;
}

/*Moves active tetromino left by one on the board.*/
void Board::shiftBlocksLeft() {
	bool isFirstShift = true;
	int lowerBound = t_minI, upperBound = t_maxI, leftBound = t_minJ, rightBound = t_maxJ;

	for (int i = lowerBound; i <= upperBound; i++) {
		for (int j = leftBound; j <= rightBound; j++) {
			if (j > 0 && board[i][j].getState() == 1 && board[i][j - 1].getValue() == 0) {
				swap(board, i, j, i, j - 1);
				if (isFirstShift) {
					t_minJ--;
					t_maxJ--;
					gt_minJ--;
					gt_maxJ--;
					isFirstShift = false;
				}
			}

		}

	}
	//if (getMoveDownCollisionBool(1)) {
	//	resetBoardState(1);
	//	addTetromino();
	//}
	if (!isFirstShift)
		createGhostTetromino();
}

/*Moves active tetromino right by one on the board.*/
void Board::shiftBlocksRight() {
	bool isFirstShift = true;
	int lowerBound = t_minI, upperBound = t_maxI, leftBound = t_minJ, rightBound = t_maxJ;

	for (int i = upperBound; i >= lowerBound; i--) {
		for (int j = rightBound; j >= leftBound; j--) {
			if (j < 9 && board[i][j].getState() == 1 && board[i][j + 1].getValue() == 0) {
				swap(board, i, j, i, j + 1);
				if (isFirstShift) {
					t_minJ++;
					t_maxJ++;
					gt_minJ++;
					gt_maxJ++;
					isFirstShift = false;
				}
			}
		}

	}
	//if (getMoveDownCollisionBool(1)) {
	//	resetBoardState(1);
	//	addTetromino();
	//}
	if (!isFirstShift)
		createGhostTetromino();
}

void Board::updateScore(int removedRowCount) {
	if (removedRowCount == 4)
	{
		score += 1000 * getLevel();
	}
	else if (removedRowCount == 3)
	{
		score += 350 * getLevel();
	}
	else if (removedRowCount == 2)
	{
		score += 150 * getLevel();
	}
	else
	{
		score += 50 * getLevel();
	}
}

/*Searches for full rows, if found removes row via function call*/
void Board::checkForFullRows() {
	int removedRowCount = 0;
	for (int i = t_maxI; i >= t_minI; i--) {
		int blockCount = 0;
		for (int j = board[i].size() - 1; j >= 0; j--) {
			if (board[i][j].getValue() > 0)
				blockCount++;
		}
		if (blockCount == 10) {
			removedRowCount++;
			removeRow(i);
			i++;
		}
	}
	if (removedRowCount > 0) {
		updateScore(removedRowCount);
		rowsRemoved += removedRowCount;
	}
}

/*Removes row and shifts all above rows down.*/
void Board::removeRow(int row) {
	for (int i = row; i >= 0; i--) {
		for (int j = 0; j < board[i].size(); j++) {
			if (i == row) {
				board[i][j].setValue(0);
				board[i][j].setState(0);
			}
			else if (board[i][j].getValue() > 0) {
				board[i + 1][j].setValue(board[i][j].getValue());
				board[i + 1][j].setState(0);
				board[i][j].setValue(0);
				board[i][j].setState(0);
			}
		}
	}
}

void Board::updateFullRowState(int i) {
	for (int j = 0; j < board[i].size(); j++) {
		board[i][j].setState(3);
	}
}

vector<int> Board::getPivotPoint(int t_id) {
	vector<int> center;
	int iSpan = t_maxI - t_minI;
	int jSpan = t_maxJ - t_minJ;

	if (t_id == 2) {
		if (jSpan == 3)
			center = { t_minI, t_maxJ - 1 };
		else
			center = { t_minI + 1, t_minJ };
	}
	else if (t_id == 3 || t_id == 4) {
		if (jSpan == 2)
			center = { t_minI, t_minJ + 1 };
		else
			center = { t_minI + 1, t_minJ };
	}
	else {
		if (jSpan == 2) {
			if (board[t_minI][t_minJ].getValue() > 0 && board[t_minI][t_minJ + 1].getValue() > 0 && board[t_minI][t_maxJ].getValue() > 0)
				center = { t_minI, t_minJ + 1 };
			else
				center = { t_minI + 1, t_minJ + 1 };
		}
		else {
			if (board[t_minI][t_minJ].getValue() > 0 && board[t_minI + 1][t_minJ].getValue() > 0 && board[t_maxI][t_minJ].getValue() > 0)
				center = { t_minI + 1, t_minJ };
			else
				center = { t_minI + 1, t_minJ + 1 };
		}
	}
	return center;
}

void Board::rotateTetromino()
{
	int t_id = getTetrominoId();
	vector<int> center = getPivotPoint(t_id);
	int minI = center[0] - 1, maxI = center[0] + 2, minJ = center[1] - 2, maxJ = center[1] + 1;
	int iSpan = t_maxI - t_minI;
	int jSpan = t_maxJ - t_minJ;

	if (t_id == 2) {
		if (getRotationCollisionBool(minI, maxI, minJ, maxJ))
			return;
	}
	minJ++;
	maxI--;

	if (getRotationCollisionBool(minI, maxI, minJ, maxJ))
		return;
	if (t_id >= 2 && t_id <= 4) {
		if (iSpan > jSpan)
			rotateRight(t_id, minI, maxI, minJ, maxJ);
		else
			rotateLeft(t_id, minI, maxI, minJ, maxJ);
	}
	else if (t_id >= 5) {
		rotateLeft(t_id, minI, maxI, minJ, maxJ);
	}

	updateMinMax();
	createGhostTetromino();
}

void Board::rotateLeft(int t_id, int minI, int maxI, int minJ, int maxJ) {
	//test if spot being moved to contains a block
	bool test1 = testValueAndState(minI, minJ, 0, 0);
	bool test2 = testValueAndState(minI, maxJ, 0, 0);
	bool test3 = testValueAndState(maxI, maxJ, 0, 0);
	bool test4 = testValueAndState(maxI, minJ, 0, 0);
	bool test5 = testValueAndState(minI, minJ + 1, 0, 0);
	bool test6 = testValueAndState(minI + 1, maxJ, 0, 0);
	bool test7 = testValueAndState(maxI, minJ + 1, 0, 0);
	bool test8 = testValueAndState(minI + 1, minJ, 0, 0);

	//test I block if required
	bool test9 = false;
	bool test10 = false;
	if (t_id == 2) {
		test9 = testValueAndState(maxI + 1, maxJ - 1, 0, 0);
	}

	//if a collision is detected return
	if (test1 || test2 || test3 || test4 || test5 || test6 || test7 || test8 || test9 || test10)
		return;

	//swap I outer block spots if required
	if (t_id == 2) {
		Block temp = board[minI + 1][minJ - 1];
		board[minI + 1][minJ - 1] = board[maxI + 1][maxJ - 1];
		board[maxI + 1][maxJ - 1] = temp;
	}

	//rotate 9X9 array - 1st swap 4 corners then swap 4 inner blocks
	Block temp = board[minI][minJ];
	board[minI][minJ] = board[minI][maxJ];
	board[minI][maxJ] = board[maxI][maxJ];
	board[maxI][maxJ] = board[maxI][minJ];
	board[maxI][minJ] = temp;


	temp = board[minI][minJ + 1];
	board[minI][minJ + 1] = board[minI + 1][maxJ];
	board[minI + 1][maxJ] = board[maxI][minJ + 1];
	board[maxI][minJ + 1] = board[minI + 1][minJ];
	board[minI + 1][minJ] = temp;
}

void Board::rotateRight(int t_id, int minI, int maxI, int minJ, int maxJ) {
	//test if spot being moved to contains a block
	bool test1 = testValueAndState(minI, minJ, 0, 0);
	bool test2 = testValueAndState(maxI, minJ, 0, 0);
	bool test3 = testValueAndState(maxI, maxJ, 0, 0);
	bool test4 = testValueAndState(minI, maxJ, 0, 0);
	bool test5 = testValueAndState(minI + 1, minJ, 0, 0);
	bool test6 = testValueAndState(maxI, minJ + 1, 0, 0);
	bool test7 = testValueAndState(minI + 1, maxJ, 0, 0);
	bool test8 = testValueAndState(minI, minJ + 1, 0, 0);

	//test I block if required
	bool test9 = false;
	bool test10 = false;
	if (t_id == 2) {
		test9 = testValueAndState(minI + 1, minJ - 1, 0, 0);
	}

	//if a collision is detected return
	if (test1 || test2 || test3 || test4 || test5 || test6 || test7 || test8 || test9 || test10)
		return;

	//swap I outer block spots if required
	if (t_id == 2) {
		Block temp = board[minI + 1][minJ - 1];
		board[minI + 1][minJ - 1] = board[maxI + 1][maxJ - 1];
		board[maxI + 1][maxJ - 1] = temp;
	}

	//rotate 9X9 array - 1st swap 4 corners then swap 4 inner blocks
	Block temp = board[minI][minJ];
	board[minI][minJ] = board[maxI][minJ];
	board[maxI][minJ] = board[maxI][maxJ];
	board[maxI][maxJ] = board[minI][maxJ];
	board[minI][maxJ] = temp;

	temp = board[minI + 1][minJ];
	board[minI + 1][minJ] = board[maxI][minJ + 1];
	board[maxI][minJ + 1] = board[minI + 1][maxJ];
	board[minI + 1][maxJ] = board[minI][minJ + 1];
	board[minI][minJ + 1] = temp;
}

bool Board::getRotationCollisionBool(int minI, int maxI, int minJ, int maxJ) {
	if (minI < 0 || maxI > 19 || minJ < 0 || maxJ > 9)
		return true;

	return false;
}

bool Board::testValueAndState(int i, int j, int value, int state) {
	return board[i][j].getValue() > value&& board[i][j].getState() == state;
}

void Board::updateMinMax() {
	int minI = 19, maxI = 0, minJ = 9, maxJ = 0;
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j].getState() == 1) {
				if (i < minI)
					minI = i;
				if (i > maxI)
					maxI = i;
				if (j < minJ)
					minJ = j;
				if (j > maxJ)
					maxJ = j;
			}
		}
	}
	t_minI = minI, t_maxI = maxI, t_minJ = minJ, t_maxJ = maxJ;
}

void Board::removeTetromino() {
	for (int i = t_minI; i <= t_maxI; i++) {
		for (int j = t_minJ; j <= t_maxJ; j++) {
			if (board[i][j].getValue() > 0 && board[i][j].getState() == 1) {
				board[i][j].setValue(0);
				board[i][j].setState(0);
			}
		}
	}
}

void Board::storePiece() {
	
	if (!hasStoredBeenUsed) {
		if (storedPiece.getTetrominoId() == 0) {
			storedPiece = tempStoredPiece;
			removeTetromino();
			addTetromino(1);
		}
		else {
			removeTetromino();
			addTetromino(2);
			storedPiece = tempStoredPiece;
			hasStoredBeenUsed = true;
		}
	}
}

void Board::shiftBlocksUp(int row) {
	for (int i = 1; i <= row; i++) {
		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j].getValue() > 0 && board[i][j].getState() == 0) {
				swap(board, i, j, i - 1, j);
			}
		}
	}
}

void Board::addDeadBlockRow() {
	
	int row = board.size() - getLevel() + 1;
	int rowCount = getLevel() / 2 + 1;
	for (int k = 19; k > 19 - rowCount; k--) {
			shiftBlocksUp(k);
		for (int i = k; i >= k; i--) {
			int notDead = rand() % 10;
			for (int j = board[i].size() - 1; j >= 0; j--) {
				if (j != notDead) {
					board[i][j].setValue(8);
					board[i][j].setState(0);
				}
			}
		}
	}
	createGhostTetromino();
}