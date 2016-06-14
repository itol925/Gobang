#include "JChessBoard.h"
#include "chessboard.h"
#include "AI.h"
#include <stdlib.h>

ChessBoard m_chessboard;
AI m_AI;
/*
 * Class:     edu_guet_gobang_Chessboard
 * Method:    AddPiece
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_guet_pyl_gobang_Chessboard_AddPiece(JNIEnv * env, jobject obj, jint _row, jint _col)
{
	if(m_chessboard.AddPiece(_row, _col))
	{
		return 1;
	}
	return 0;
}

/*
 * Class:     edu_guet_gobang_Chessboard
 * Method:    StartGame
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guet_pyl_gobang_Chessboard_StartGame(JNIEnv * env, jobject obj)
{
	return 1;
}

/*
 * Class:     edu_guet_gobang_Chessboard
 * Method:    NextMove
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guet_pyl_gobang_Chessboard_NextMove(JNIEnv * env, jobject obj)
{
	if(m_AI.NextMove(&m_chessboard)){
		return 1;
	}
	return 0;
}

/*
 * Class:     edu_guet_gobang_Chessboard
 * Method:    CheckGameOver
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guet_pyl_gobang_Chessboard_CheckGameOver(JNIEnv * env, jobject obj)
{
	return m_chessboard.CheckGameOver();
}

/*
 * Class:     edu_guet_gobang_Chessboard
 * Method:    GetGrid
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_guet_pyl_gobang_Chessboard_GetGrid(JNIEnv *, jobject obj, jint _row, jint _col)
{
	Grid * grid = m_chessboard.Grids(_row, _col);
	if(!grid)
		return 0;
	if(grid->party == FREE)
		return 1;
	else if(grid->party == BLACK)
		return 2;
	else if(grid->party == WHITE)
		return 3;
	return 0;
}

/*
 * Class:     edu_guet_gobang_Chessboard
 * Method:    ClearChessBoard
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guet_pyl_gobang_Chessboard_ClearChessBoard(JNIEnv *, jobject)
{
	m_chessboard.Clear();
}

/*
 * Class:     guet_pyl_gobang_Chessboard
 * Method:    GetLastStep
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_guet_pyl_gobang_Chessboard_GetLastStep(JNIEnv * env, jobject obj)
{
	jintArray iarr;
	iarr = env->NewIntArray(2);
	if( m_chessboard.GetLastRow() < 0 ||  m_chessboard.GetLastCol() < 0)
		return NULL;
	jint tmp[2];
	tmp[0] = m_chessboard.GetLastRow();
	tmp[1] = m_chessboard.GetLastCol();
	env->SetIntArrayRegion(iarr, 0, 2, tmp);
	return iarr;
}


/*
 * Class:     edu_guet_gobang_Chessboard
 * Method:    GetStepsCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guet_pyl_gobang_Chessboard_GetStepsCount(JNIEnv *, jobject)
{
	return m_chessboard.GetStepsCount();
}

/*
 * Class:     edu_guet_gobang_Chessboard
 * Method:    GetBoardSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guet_pyl_gobang_Chessboard_GetBoardSize(JNIEnv *, jobject)
{
	return m_chessboard.GetBoardSize();
}

/*
 * Class:     edu_guet_gobang_Chessboard
 * Method:    GetChengWu
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_guet_pyl_gobang_Chessboard_GetChengWu(JNIEnv * env, jobject obj)
{
	jintArray iarr;
	iarr = env->NewIntArray(10);

	jint tmp[10];
	Grid * head = m_chessboard.ChengWuHead;
	if(!head)
		return NULL;
	int count = 0;
	while(head)
	{
		tmp[count] = head->rowIndex;
		tmp[count + 1] = head->colIndex;
		head = m_chessboard.GetLineNext(m_chessboard.ChengWuLine, head);
		count += 2;
		if(count >= 10)
			break;
	}
	env->SetIntArrayRegion(iarr, 0, 10, tmp);
	return iarr;
}

JNIEXPORT void JNICALL Java_guet_pyl_gobang_Chessboard_SetBoardSize(JNIEnv *, jobject, jint size)
{
	m_chessboard.SetBoardSize(size);
}

JNIEXPORT void JNICALL Java_guet_pyl_gobang_Chessboard_SetDiffcult(JNIEnv *, jobject, jint diff)
{
	m_AI.SetDiffcult(diff);
}
