#include "chessboard.h"
#include "ai.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define HARDEST 10

AI::AI()
{
	srand((unsigned)time(NULL));
	m_diffcult = HARDEST;	//默认为最难
}
AI::~AI()
{}
bool AI::NextMove(ChessBoard* chessBoard)
{
	int nextRowIndex, nextColIndex;
	/******** AI执黑棋，下第一步棋，随机一步 *****************************************/
	int stepsCount = chessBoard->GetStepsCount();
	if(stepsCount == 0)
	{
		int s = chessBoard->GetBoardSize() / 3, e = chessBoard->GetBoardSize() - s;
		nextRowIndex = s + (rand()%(e - s));
		nextColIndex = s + (rand()%(e - s));
		return chessBoard->AddPiece(nextRowIndex, nextColIndex);
	}
	/*********************************************************************************/

	/******** 确定AI和对手的棋色 *****************************************************/
	Party curPar, oppoPar;
	if(stepsCount % 2 == 0){
		curPar = BLACK;
		oppoPar = WHITE;
	}else{
		curPar = WHITE;
		oppoPar = BLACK;
	}
	/*********************************************************************************/

	/******** 重新评估对方权重，先清零，再评估 ***************************************/
	Evaluate(chessBoard, oppoPar);
	/*********************************************************************************/

	/******* 玩家执黑棋，AI执白棋，玩家下完第一步棋，AI下第二步 **********************/
	if(stepsCount == 1)
	{
		if(RandomABestMove(chessBoard, oppoPar, nextRowIndex, nextColIndex))
			return chessBoard->AddPiece(nextRowIndex, nextColIndex);
		return false;
	}
	/*********************************************************************************/

	/*************** 对方已落子，重新评估己方权重，先清零再评估 **********************/
	Evaluate(chessBoard, curPar);
	/*********************************************************************************/

	/*************************** 获取最佳位置 ****************************************/
	if(RandomABestMove(chessBoard, curPar, nextRowIndex, nextColIndex))
		return chessBoard->AddPiece(nextRowIndex, nextColIndex);
	return false;
	/*********************************************************************************/
}

/*************************************************
*评估棋色为par的棋局的权重值
*************************************************/
void AI::Evaluate(ChessBoard* chessBoard, Party par)
{
	//将棋色为par的权重清0
	int i;
	for(i = 0; i < chessBoard->GetBoardSize(); i++){
		for(int j = 0; j < chessBoard->GetBoardSize(); j++){
			chessBoard->Grids(i, j)->SetWeight(par, 0);
		}
	}
	int boardSize = chessBoard->GetBoardSize();
	//评估权重
	for(i = 0; i <= boardSize - 1; i++){
		SetLineWeight(chessBoard, par, L_R, i, boardSize);		//L_R
	}
	for(i = 0; i <= boardSize - 1; i++){
		SetLineWeight(chessBoard, par, U_D, i, boardSize);		//U_D
	}
	for(i = 0 - boardSize + 1; i <= boardSize - 1; i++){
		int res = i > 0 ? i : (0 - i);
		res = boardSize - res;
		SetLineWeight(chessBoard, par, LU_RD, i, res);			//LU_RD
	}
	for(i = 0; i <= boardSize - 1 + boardSize - 1; i++){
		int res = boardSize;
		if(i >= boardSize)
			res = 2 * boardSize - 1;
		SetLineWeight(chessBoard, par, LD_RU, i, res);			//LD_RU
	}
}
void AI::SetLineWeight(ChessBoard* chessBoard, Party par, Line li, int lineIndex, int lineSize)
{
	Grid** lineGrid = new Grid*[lineSize];
	char* lineStr = new char[lineSize + 2];
	lineStr[0] = lineStr[lineSize + 1] = 'x';		//行的开始和末尾都为x

	int parCount = 0;					//该行本方棋子的个数
	int index = 1;
	Grid* lastGrid = chessBoard->GetLineBegin(li, lineIndex);
	while(lastGrid){
		lineGrid[index - 1] = lastGrid;
		if(lastGrid->party == par){
			parCount++;
			lineStr[index] = 'o';		//当是本方棋色时为o
		}
		else if(lastGrid->party == FREE){
			lineStr[index] = '_';		//当是空位是为_
		}
		else{
			lineStr[index] = 'x';		//当是对方棋色或边界时为x
		}
		lastGrid = chessBoard->GetLineNext(li, lastGrid);
		index++;
	}

	if(parCount >= 4){
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, CHENG_WU);		
	}
	if(parCount >= 3){
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, LIAN_HUO_SI);
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, LIAN_MIAN_SI);
	}
	if(parCount >= 2){
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, LIAN_HUO_SAN);
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, LIAN_MIAN_SAN);
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, TIAO_HUO_SAN);
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, TIAO_MIAN_SAN);
	}
	if(parCount >= 1){
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, LIAN_HUO_ER);
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, LIAN_MIAN_ER);
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, TIAO_HUO_ER);
		SetGridWeight(lineGrid, lineStr, lineSize + 2, par, TIAO_MIAN_ER);
	}
	delete[] lineGrid;
	delete[] lineStr;
}
void AI::SetGridWeight(Grid ** lineGrid, char* lineStr, int lineLength, Party par, GridWeight weight)
{	
	switch(weight)
	{
	case CHENG_WU:
		{
			if(lineLength < 7)
				return;
			char * model1 = "_oooo";
			char * model2 = "o_ooo";
			char * model3 = "oo_oo";
			char * model4 = "ooo_o";
			char * model5 = "oooo_";
			MatchAndWeight(lineStr, lineLength, model1, 5, lineGrid, 0, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 5, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model3, 5, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model4, 5, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, model5, 5, lineGrid, 4, par, weight);
			return;
		}
	case LIAN_HUO_SI:
		{
			if(lineLength < 8)
				return;
			char * model1 = "__ooo_";
			char * model2 = "_o_oo_";
			char * model3 = "_oo_o_";
			char * model4 = "_ooo__";
			MatchAndWeight(lineStr, lineLength, model1, 6, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 6, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model3, 6, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, model4, 6, lineGrid, 4, par, weight);
			return;
		}
	case LIAN_MIAN_SI:
		{
			if(lineLength < 8)
				return;
			char * model1 = "x_ooo_";
			char * model2 = "xo_oo_";
			char * model3 = "xoo_o_";
			char * model4 = "xooo__";
			char * model5 = "__ooox";
			char * model6 = "_o_oox";
			char * model7 = "_oo_ox";
			char * model8 = "_ooo_x";
			MatchAndWeight(lineStr, lineLength, model1, 6, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 6, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model3, 6, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, model4, 6, lineGrid, 4, par, weight);
			MatchAndWeight(lineStr, lineLength, model5, 6, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model6, 6, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model7, 6, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, model8, 6, lineGrid, 4, par, weight);
			return;
		}
	case LIAN_HUO_SAN:
		{
			if(lineLength < 7)
				return;
			char * model1 = "__oo_";
			char * model2 = "_o_o_";
			char * model3 = "_oo__";
			MatchAndWeight(lineStr, lineLength, model1, 5, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 5, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model3, 5, lineGrid, 3, par, weight);
			return;
		}
	case LIAN_MIAN_SAN:
		{
			if(lineLength < 8)
				return;
			char * model1 = "x_oo__";	//添加一个成五判断位
			char * model2 = "xo_o__";
			char * model3 = "xoo___";
			char * model4 = "___oox";
			char * model5 = "__o_ox";
			char * model6 = "__oo_x";
			MatchAndWeight(lineStr, lineLength, model1, 6, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 6, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model3, 6, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, model4, 6, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model5, 6, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, model6, 6, lineGrid, 4, par, weight);
			return;
		}
	case TIAO_HUO_SAN:
		{
			if(lineLength < 8)
				return;
			char * model1 = "__o_o_";
			char * model2 = "_o_o__";
			char * model3 = "_o__o_";
			char * model4 = "_o__o_";	//虽然与上一行相同，但thisIndex不同
			char * model5 = "_oo___";
			char * model6 = "___oo_";
			MatchAndWeight(lineStr, lineLength, model1, 6, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 6, lineGrid, 4, par, weight);
			MatchAndWeight(lineStr, lineLength, model3, 6, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model4, 6, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, model5, 6, lineGrid, 4, par, weight);
			MatchAndWeight(lineStr, lineLength, model6, 6, lineGrid, 1, par, weight);
			return;
		}
	case TIAO_MIAN_SAN:
		{
			if(lineLength < 8)
				return;
			char * model1 = "x_o_o_";
			char * model2 = "xo_o__";
			char * model3 = "xo__o_";
			char * model4 = "xo__o_";	//虽然与上一行相同，但thisIndex不同
			char * model5 = "xoo___";
			char * model6 = "x__oo_";
			char * model7 = "__o_ox";
			char * model8 = "_o_o_x";
			char * model9 = "_o__ox";
			char * modelA = "_o__ox";
			char * modelB = "_oo__x";
			char * modelC = "___oox";
			MatchAndWeight(lineStr, lineLength, model1, 6, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 6, lineGrid, 4, par, weight);
			MatchAndWeight(lineStr, lineLength, model3, 6, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model4, 6, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, model5, 6, lineGrid, 4, par, weight);
			MatchAndWeight(lineStr, lineLength, model6, 6, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model7, 6, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model8, 6, lineGrid, 4, par, weight);
			MatchAndWeight(lineStr, lineLength, model9, 6, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, modelA, 6, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, modelB, 6, lineGrid, 4, par, weight);
			MatchAndWeight(lineStr, lineLength, modelC, 6, lineGrid, 1, par, weight);
			return;
		}
	case LIAN_HUO_ER:
		{
			if(lineLength < 7)
				return;
			char * model1 = "__o__";	//添加1个成五判断位
			char * model2 = "_o___";
			char * model3 = "___o_";
			char * model4 = "__o__";
			MatchAndWeight(lineStr, lineLength, model1, 5, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 5, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model3, 5, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model4, 5, lineGrid, 3, par, weight);
			return;
		}
	case LIAN_MIAN_ER:
		{
			if(lineLength < 8)
				return;
			char * model1 = "x_o___";	//添加2个成五判断位
			char * model2 = "xo____";
			char * model3 = "____ox";
			char * model4 = "___o_x";
			MatchAndWeight(lineStr, lineLength, model1, 6, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 6, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model3, 6, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, model4, 6, lineGrid, 4, par, weight);
			return;
		}
	case TIAO_HUO_ER:
		{
			if(lineLength < 7)
				return;
			char * model1 = "___o_";
			char * model2 = "_o___";
			MatchAndWeight(lineStr, lineLength, model1, 5, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 5, lineGrid, 3, par, weight);
			return;
		}
	case TIAO_MIAN_ER:
		{
			if(lineLength < 8)
				return;
			char * model1 = "x__o__";	//添加一个成五判断位
			char * model2 = "xo____";
			char * model3 = "____ox";
			char * model4 = "__o__x";
			MatchAndWeight(lineStr, lineLength, model1, 6, lineGrid, 1, par, weight);
			MatchAndWeight(lineStr, lineLength, model2, 6, lineGrid, 3, par, weight);
			MatchAndWeight(lineStr, lineLength, model3, 6, lineGrid, 2, par, weight);
			MatchAndWeight(lineStr, lineLength, model4, 6, lineGrid, 4, par, weight);
			return;
		}
	}
}
void AI::MatchAndWeight(char* lineStr, int lineLength, char* model, int modelLength, Grid ** lineGrid, int thisIndex, Party par, GridWeight weight)
{
	int i, j;
	for(i = 0; i < lineLength; i++)
	{
		if(weight != CHENG_WU && rand() % HARDEST > this->m_diffcult)	//控制AI难度，让AI出现漏评的情况
			continue;
		for(j = 0; j < modelLength; j++)
		{
			if((i + j) < lineLength && model[j] == lineStr[i + j])
				continue;
			break;
		}
		if(j == modelLength)
			(lineGrid[i - 1 + thisIndex])->SetWeight_Add(par, weight);
	}
}
bool AI::RandomABestMove(ChessBoard* chessBoard, Party par, int& nextRowIndex, int& nextColIndex)
{
	int maxWeight = -10000, maxWeightCount = 0;
	int i, j;
	for(i = 0; i < chessBoard->GetBoardSize(); i++){
		for(j = 0; j < chessBoard->GetBoardSize(); j++){
			int wei = chessBoard->Grids(i, j)->GetComposWeight(par);
			if(wei >= CHENG_WU){		//判断是否有成五，有则返回该点，游戏结束
				nextRowIndex = i;
				nextColIndex = j;
				return true;
			}
			if(wei > maxWeight){		//没有成五的时候，寻找最大权重值
				maxWeight = wei;
				maxWeightCount = 0;
			}
			if(wei == maxWeight)		//统计最大值的个数
				maxWeightCount++;
		}
	}
	int randIndex = rand() % maxWeightCount;
	int index = 0;
	for(i = 0; i < chessBoard->GetBoardSize(); i++){
		for(j = 0; j < chessBoard->GetBoardSize(); j++){
			if(chessBoard->Grids(i, j)->GetComposWeight(par) == maxWeight && chessBoard->Grids(i, j)->party == FREE){
				nextRowIndex = i;
				nextColIndex = j;
				if(index == randIndex)
					return true;
				index++;
			}
		}
	}
	//上面的方法并没有找到一个合适的点位时，随便找个空位下了
	if(chessBoard->Grids(i, j) == NULL)
		for(i = 0; i < chessBoard->GetBoardSize(); i++){
			for(j = 0; j < chessBoard->GetBoardSize(); j++){
				if(chessBoard->Grids(i, j)->party == FREE){
					nextRowIndex = i;
					nextColIndex = j;
					return true;
				}
			}
		}
	return false;
}
void AI::Clear()
{
}
void AI::SetDiffcult(int diffcult)
{
	if(diffcult >= 1 && diffcult <= HARDEST)
		this->m_diffcult = diffcult;
}
