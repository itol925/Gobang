#ifndef AI_H_H_H
#define AI_H_H_H

#include "definitions.h"

class ChessBoard;
class AI
{
public:
	AI();
	~AI();
public:
	bool NextMove(ChessBoard* chessBoard);
	void Clear();
	void SetDiffcult(int diffcult);
private:
	int m_diffcult;
private:
	void Evaluate(ChessBoard* chessBoard, Party par);
	void SetLineWeight(ChessBoard* chessBoard, Party par, Line li, int lineIndex, int lineSize);
	void SetGridWeight(Grid ** lineGrid, char* lineStr, int lineLength, Party par, GridWeight weight);
	void MatchAndWeight(char* lineStr, int lineLength, char* model, int modelLength, Grid ** lineGrid, int thisIndex, Party par, GridWeight weight);

	bool RandomABestMove(ChessBoard* chessBoard, Party par, int& nextRowIndex, int& nextColIndex);
};

#endif