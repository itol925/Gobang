#ifndef STRUCTANDENUM_H_H_H
#define STRUCTANDENUM_H_H_H
enum GridWeight
{
	CHENG_WU		=	10000,			// ����һ��������

	LIAN_HUO_SI		=	2000,			// ������
	LIAN_MIAN_SI	=	401,			// ������

	LIAN_HUO_SAN	=	400,			// ������
	TIAO_HUO_SAN	=	300,			// ������
	LIAN_MIAN_SAN	=	50,				// ������
	TIAO_MIAN_SAN	=	30,				// ������

	LIAN_HUO_ER		=	10,				// �����
	TIAO_HUO_ER		=	8,				// �����
	LIAN_MIAN_ER	=	2,				// ���߶�
	TIAO_MIAN_ER	=	1				// ���߶�
};
enum Party{FREE,BLACK,WHITE};
enum Line{L_R,U_D,LU_RD,LD_RU};			//�ֱ���� ��-��, ��-��, ����-����, ����-�����ĸ��������
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

		curScale = 1.0f;	//�������ӣ����Ա�������Ϊ��Ҫ�ο����ӣ�����Ϊ1
		oppoScale = 0.4f;	//�������ӣ����ԶԷ�����Ϊ��Ҫ�ο�����
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
	int comWeightForBLACK, comWeightForWHITE;//�ۺ�Ȩֵ
	float curScale, oppoScale;
};
#endif
