#ifndef CHESSBOARD_H_H
#define CHESSBOARD_H_H

#include "definitions.h"

class ChessBoard
{
private:
	int m_BoardSize;
	//int m_gridSize;
	//int m_pieceSize;
	//POINT m_leftTop;
	Grid *m_chessboard;
	int m_stepsCount;	//���������ӵĸ���
	int lastStepRow, lastStepCol;
public:
	Grid * GetLineBegin(Line li,int lineIndex);
	Grid * GetLineNext(Line li, Grid * lastGrid);
	Grid * ChengWuHead;	//����ʱ�ĵ�һ����
	Line ChengWuLine;
private:
	bool CheckLine(Line li, int lineIndex, Party par);
public:
	int GetStepsCount()	{	return m_stepsCount;}
	int GetBoardSize()	{	return m_BoardSize;	}
	//int GetGridSize()	{	return m_gridSize;	}
	//int GetPieceSize()	{	return m_pieceSize;	}
	//POINT GetStartPos()	{	return m_leftTop;	}
	int GetLastRow()	{	return lastStepRow;	}
	int GetLastCol()	{	return lastStepCol;	}
	//void SetGridSize(int size)	{	m_gridSize = size;	}
	//void SetPieceSize(int size)	{	m_pieceSize = size;	}
	//void SetStartPos(POINT pt)	{	m_leftTop = pt;		}
	Grid * Grids(int rowIndex, int colIndex);
	//RECT GetRect();		//�������̵ķ�Χ

	//bool AddPiece(POINT clickPt);
	bool AddPiece(int row, int col);
	int CheckGameOver();
	void Clear();
	void SetBoardSize(int size);
public:
	ChessBoard();
	~ChessBoard();
};
#endif
