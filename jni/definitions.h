#ifndef STRUCTANDENUM_H_H_H
#define STRUCTANDENUM_H_H_H
enum GridWeight
{
	CHENG_WU		=	10000,			// 再走一步则五连

	LIAN_HUO_SI		=	2000,			// 连活四
	LIAN_MIAN_SI	=	401,			// 连眠四

	LIAN_HUO_SAN	=	400,			// 连活三
	TIAO_HUO_SAN	=	300,			// 跳活三
	LIAN_MIAN_SAN	=	50,				// 连眠三
	TIAO_MIAN_SAN	=	30,				// 跳眠三

	LIAN_HUO_ER		=	10,				// 连活二
	TIAO_HUO_ER		=	8,				// 跳活二
	LIAN_MIAN_ER	=	2,				// 连眠二
	TIAO_MIAN_ER	=	1				// 跳眠二
};
enum Party{FREE,BLACK,WHITE};
enum Line{L_R,U_D,LU_RD,LD_RU};			//分别代表 左-右, 上-下, 左上-右下, 左下-右上四个方向的线
class Grid
{
public:
	Grid()
	{
		rowIndex = -1;
		colIndex = -1;

		party = FREE;
		weightForBLACK = weightForWHITE = 0;
		comWeightForBLACK = comWeightForWHITE = 0;
		//DeepWeight = 0;

		curScale = 1.0f;	//进攻因子，即以本方棋子为主要参考落子，建议为1
		oppoScale = 0.4f;	//防守因子，即以对方棋子为主要参考落子
	}
public:
	Party party;
	int rowIndex;
	int colIndex;
	int GetWeight(Party par)
	{
		if(par == BLACK)
			return weightForBLACK;
		else if(par == WHITE)
			return weightForWHITE;
		return 0;
	}
	void SetWeight(Party par, int wei)
	{
		if(par == BLACK)
			weightForBLACK = wei;
		else if(par == WHITE)
			weightForWHITE = wei;
	}
	void SetWeight_Add(Party par, int wei)
	{
		if(par == BLACK)
			weightForBLACK += wei;
		else if(par == WHITE)
			weightForWHITE += wei;
	}
	int GetComposWeight(Party par)
	{
		if(par == BLACK)
			return (int)(weightForBLACK * curScale + weightForWHITE * oppoScale);
		else if(par == WHITE)
			return (int)(weightForBLACK * oppoScale + weightForWHITE * curScale);
		return 0;
	}
	void SetOppoScale(float o)
	{
		oppoScale = o;
	}
private:
	int weightForBLACK, weightForWHITE;
	int comWeightForBLACK, comWeightForWHITE;//综合权值
	float curScale, oppoScale;
};
#endif
