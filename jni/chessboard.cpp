#include "chessboard.h"
#include "definitions.h"
#include <stdlib.h>
//#include <stdio.h>

ChessBoard::ChessBoard()
{
	m_stepsCount = 0;
	m_BoardSize = 15;
	//m_gridSize = 30;
	//m_pieceSize = 20;
	//m_leftTop.x = m_leftTop.y = 10;
	lastStepRow = lastStepCol = -1;
	ChengWuHead = NULL;

	m_chessboard = new Grid[m_BoardSize * m_BoardSize];
	for(int i = 0; i < m_BoardSize; i++)
	{
		for(int j = 0; j < m_BoardSize; j++)
		{
			m_chessboard[i * m_BoardSize + j].rowIndex = i;
			m_chessboard[i * m_BoardSize + j].colIndex = j;
		}
	}
}
ChessBoard::~ChessBoard()
{
	if(m_chessboard != NULL){
		delete [] m_chessboard;
		m_chessboard = NULL;
		//printf("release memory!");
	}
}

void ChessBoard::SetBoardSize(int size)
{
	m_BoardSize = size;
	ChessBoard();
	delete [] m_chessboard;
	m_chessboard = new Grid[m_BoardSize * m_BoardSize];
	for(int i = 0; i < m_BoardSize; i++)
	{
		for(int j = 0; j < m_BoardSize; j++)
		{
			m_chessboard[i * m_BoardSize + j].rowIndex = i;
			m_chessboard[i * m_BoardSize + j].colIndex = j;
		}
	}
}
Grid * ChessBoard::Grids(int rowIndex, int colIndex)
{
	if(rowIndex >= 0 && rowIndex < m_BoardSize && colIndex >=0 && colIndex < m_BoardSize)
		return &m_chessboard[rowIndex * m_BoardSize + colIndex];
	return NULL;
}
/*bool ChessBoard::AddPiece(POINT clickPt)
{
	int row = clickPt.y - m_leftTop.y + m_gridSize/2;
	int col = clickPt.x - m_leftTop.x + m_gridSize/2;
	row /= m_gridSize;
	col /= m_gridSize;
	if(row < 0 || row >= m_BoardSize || col < 0 || col >= m_BoardSize)
		return false;
	if(Grids(row, col) == NULL)
		return false;
	if(Grids(row, col)->party != FREE)
		return false;
	if(m_stepsCount % 2 == 0)
		Grids(row, col)->party = BLACK;
	else
		Grids(row, col)->party = WHITE;
	m_stepsCount++;
	lastStepRow = Grids(row, col)->rowIndex;
	lastStepCol = Grids(row, col)->colIndex;

	return true;
}*/
bool ChessBoard::AddPiece(int row, int col)
{
	if(Grids(row, col) == NULL)
		return false;
	if(Grids(row, col)->party != FREE)
		return false;
	if(m_stepsCount % 2 == 0)
		Grids(row, col)->party = BLACK;
	else
		Grids(row, col)->party = WHITE;
	m_stepsCount++;
	lastStepRow = row;
	lastStepCol = col;
	return true;
}
/*RECT ChessBoard::GetRect()
{
	RECT rec;
	rec.left = m_leftTop.x - m_gridSize/2;
	rec.right = m_leftTop.x + (m_BoardSize - 1) * m_gridSize + m_gridSize/2;
	rec.top = m_leftTop.y - m_gridSize/2;
	rec.bottom = m_leftTop.y + (m_BoardSize - 1) * m_gridSize + m_gridSize/2;
	return rec;
}*/
void ChessBoard::Clear()
{
	m_stepsCount = 0;
	for(int i = 0; i < m_BoardSize * m_BoardSize; i++)
	{
		m_chessboard[i].SetWeight(BLACK, 0);
		m_chessboard[i].SetWeight(WHITE, 0);
		m_chessboard[i].party = FREE;
	}
	ChengWuHead = NULL;
	lastStepRow = -1;
	lastStepCol = -1;
}
//检查游戏结束与否
bool ChessBoard::CheckLine(Line li, int lineIndex, Party par)
{
	int parCount = 0;
	Grid* lastGrid = GetLineBegin(li, lineIndex);
	while(lastGrid){
		if(lastGrid->party == par){
			parCount++;
			if(ChengWuHead == NULL){
				ChengWuHead = lastGrid;
			}
		}else{
			parCount = 0;
			ChengWuHead = NULL;
		}
		if(parCount >= 5){
			ChengWuLine = li;
			return true;
		}
		lastGrid = GetLineNext(li, lastGrid);
	}
	ChengWuHead = NULL;
	return false;
}
int ChessBoard::CheckGameOver()
{
	if(m_stepsCount < 9)
		return 0;
	int steps = m_stepsCount - 1;
	Party par;
	if(steps % 2 == 0)
		par = BLACK;
	else
		par = WHITE;

	int i;
	for(i = 0; i < m_BoardSize; i++)	//从L_R方向上找
		if(CheckLine(L_R, i, par))
			return 1;
	for(i = 0; i < m_BoardSize; i++)	//从U_D方向上找
		if(CheckLine(U_D, i, par))
			return 1;
	for(i = 0 - m_BoardSize + 5; i <= m_BoardSize - 5; i++)	//从LU_RD方向上找
		if(CheckLine(LU_RD, i, par))
			return 1;
	for(i = 4; i <= m_BoardSize + m_BoardSize - 4; i++)	//从LD_RU方向上找
		if(CheckLine(LD_RU, i, par))
			return 1;
	if(steps == m_BoardSize * m_BoardSize - 1)	//和局
		return 2;
	return 0;
}

Grid * ChessBoard::GetLineBegin(Line li, int lineIndex)
{
	switch(li)
	{
	case L_R:
		return Grids(lineIndex, 0);
	case U_D:
		return Grids(0, lineIndex);
	case LU_RD:		//左上-右下方向,有负数, lineIndex = colIndex - rowIndex
		if(lineIndex < 0)
			return Grids(0 - lineIndex, 0);
		else 
			return Grids(0, lineIndex);
	case LD_RU:		//左下右上,无负数, lineIndex = colIndex + rowIndex
		if(lineIndex < m_BoardSize)
			return Grids(lineIndex, 0);
		else
			return Grids(m_BoardSize - 1, lineIndex - m_BoardSize + 1);
	}
	return NULL;
}
Grid * ChessBoard::GetLineNext(Line li, Grid * lastGrid)
{
	switch(li)
	{
	case L_R:
		return Grids(lastGrid->rowIndex, lastGrid->colIndex + 1);
	case U_D:
		return Grids(lastGrid->rowIndex + 1, lastGrid->colIndex);
	case LU_RD:		//左上-右下
		return Grids(lastGrid->rowIndex + 1, lastGrid->colIndex + 1);
	case LD_RU:		//左下-右上
		return Grids(lastGrid->rowIndex - 1, lastGrid->colIndex + 1);
	}	
	return NULL;
}
