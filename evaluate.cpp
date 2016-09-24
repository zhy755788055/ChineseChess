#include "stdafx.h"
#include "Board.h"

const short PieceValue[48] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1000,20,20,20,20,40,40,90,90,45,45,10,10,10,10,10,
	1000,20,20,20,20,40,40,90,90,45,45,10,10,10,10,10
};

short CBoard::Eval(void)
{
	int i;
	short bValue = 0;
	short wValue = 0;

	for(i = 16; i < 32; i++)
	{
		if(piece[i] > 0)
		{
			wValue = wValue + PieceValue[i];
		}
	}

	for(i = 32; i < 48; i++)
	{
		if(piece[i] > 0)
		{
			bValue = bValue + PieceValue[i];
		}
	}

	if(side)
	{
		return bValue - wValue;
	}
	else
	{
		return wValue - bValue;
	}
}
