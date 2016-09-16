#include "stdafx.h"
#include "Board.h"

//各种棋子走法数组
static short KingDir[8]		={-0x10,		-0x01,	+0x01,	+0x10,	0,		0,		0,		0};		//将
static short AdvisorDir[8]	={-0x11,		-0x0f,	+0x0f,	+0x11,	0,		0,		0,		0};		//士
static short BishopDir[8]	={-0x22,		-0x1e,	+0x1e,	+0x22,	0,		0,		0,		0};		//象
static short KnightDir[8]	={-0x21,		-0x1f,	-0x12,	-0x0e,	+0x0e,	+0x12,	+0x1f,	+0x21};	//马
static short RookDir[8]		={-0x01,		+0x01,	-0x10,	+0x10,	0,		0,		0,		0};		//车
static short CannonDir[8]	={-0x01,		+0x01,	-0x10,	+0x10,	0,		0,		0,		0};		//炮
static short PawnDir[2][8]	={																		//兵
								{-0x01,		+0x01,	-0x10,	0,		0,		0,		0,		0},
								{-0x01,		+0x01,	+0x10,	0,		0,		0,		0,		0}
							 };		

static short KnightCheck[8] = {-0x10, -0x10, -0x01, +0x01, -0x01, +0x01, +0x10, +0x10};	//马腿位置
static short BishopCheck[8] = {-0x11, -0x0f, +0x0f, +0x11, 0, 0, 0, 0};					//象眼位置
static short kingpalace[9]	= {54,55,56,70,71,72,86,87,88};								//黑方九宫位置

//各种棋子合理位置数组
static unsigned char LegalPosition[2][256] =
{
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 1,25, 1, 9, 1,25, 1, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 1, 9, 1, 9, 1, 9, 1, 9, 0, 0, 0, 0,
		0, 0, 0, 17, 1, 1, 7, 19, 7, 1, 1, 17, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 3, 7, 3, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 17, 7, 3, 7, 17, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 17, 7, 3, 7, 17, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 3, 7, 3, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 17, 1, 1, 7, 19, 7, 1, 1, 17, 0, 0, 0, 0,
		0, 0, 0, 9, 1, 9, 1, 9, 1, 9, 1, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 1,25, 1, 9, 1,25, 1, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	}
};

static unsigned char PositionMask[7] = {2, 4, 16, 1, 1, 1, 8};

//检测lSide一方是否被将军，是被将军返回1，否则返回0
int CBoard::Check(int lSide)
{
	unsigned char wKing,bKing; //红黑双方将帅的位置
	unsigned char p,q;
	int r;	//r=1表示将军，否则为0
	int SideTag = 32 - lSide * 16;	//此处表示lSide对方的将的值
	int fSide = 1-lSide;	//对方标志
	int i;
	int PosAdd;	//位置增量

	wKing = piece[16];
	bKing = piece[32];
	if(!wKing || !bKing)
		return 0;

	//检测将帅是否照面
	r=1;
	if(wKing%16 == bKing%16)
	{
		for(wKing=wKing-16; wKing!=bKing; wKing=wKing-16)
		{
			if(board[wKing])	
			{
				r=0;
				break;
			}
		}
		if(r)
			return r;	//将帅照面
	}

	q = piece[48-SideTag];	//lSide方将的位置

	//检测将是否被马攻击
	int k;
	unsigned char n;//下一步可能行走的位置
	unsigned char m;//马腿位置
	
	for(i=5;i<=6;i++)
	{
		p = piece[SideTag + i];
		if(!p)
			continue;
		for(k=0; k<8; k++)//8个方向
		{
			n = p + KnightDir[k];	//n为新的可能走到的位置
			if(n!=q)
				continue;

			if(LegalPosition[fSide][n] & PositionMask[3])	//马将对应下标为3
			{
				m = p + KnightCheck[k];
				if(!board[m])	//马腿位置无棋子占据
				{
					return 1;
				}
			}
		}
	}
	
	//检测将是否被车攻击
	r=1;
	for(i=7;i<=8;i++)
	{
		p = piece[SideTag + i];
		if(!p)
			continue;
		if(p%16 == q%16)	//在同一纵线上
		{
			PosAdd = (p>q?-16:16);
			for(p=p+PosAdd; p!=q; p = p+PosAdd)
			{
				if(board[p])	//车将中间有子隔着
				{
					r=0;
					break;
				}
			}
			if(r)
				return r;
		}
		else if(p/16 ==q/16)	//在同一横线上
		{
			PosAdd = (p>q?-1:1);
			for(p=p+PosAdd; p!=q; p = p+PosAdd)
			{
				if(board[p])
				{
					r=0;
					break;
				}
			}
			if(r)
				return r;
		}
	}
	
	//检测将是否被炮攻击
	int OverFlag = 0;	//翻山标志
	for(i=9;i<=10;i++)
	{
		p = piece[SideTag + i];
		if(!p)
			continue;
		if(p%16 == q%16)	//在同一纵线上
		{
			PosAdd = (p>q?-16:16);
			for(p=p+PosAdd; p!=q; p = p+PosAdd)
			{
				if(board[p])
				{
					if(!OverFlag)	//隔一子
						OverFlag = 1;
					else			//隔两子
					{
						OverFlag = 2;
						break;
					}
				}
			}
			if(OverFlag==1)
				return 1;
		}
		else if(p/16 ==q/16)	//在同一横线上
		{
			PosAdd = (p>q?-1:1);
			for(p=p+PosAdd; p!=q; p = p+PosAdd)
			{
				if(board[p])
				{
					if(!OverFlag)
						OverFlag = 1;
					else
					{
						OverFlag = 2;
						break;
					}
				}
			}
			if(OverFlag==1)
				return 1;
		}
	}

	//检测将是否被兵攻击
	for(i=11;i<=15;i++)
	{
		p = piece[SideTag + i];
		if(!p)
			continue;
		for(k=0; k<3; k++)//3个方向
		{
			n = p + PawnDir[fSide][k];	//n为新的可能走到的位置
			if((n==q) && (LegalPosition[fSide][n] & PositionMask[6]))	//兵士将对应下标为6
			{
				return 1;
			}
		}
	}

	return 0;
}

int CBoard::SaveMove(unsigned char from, unsigned char to,move * mv)
{
	unsigned char p;
	
	p = board[to];
	piece[board[from]] = to;
	if(p)
		piece[p]=0;
	board[to] = board[from];
	board[from] = 0;

	int r =Check(side);
	board[from] = board[to];
	board[to] = p;
	piece[board[from]] = from;
	if(p)
		piece[p] = to;

	if(!r)
	{
		mv->from = from;
		mv->to = to;
		return 1;
	}
	return 0;
}

int CBoard::GenAllMove(move * MoveArray)
{
	short i,j,k;
	unsigned char p;	//p:棋子位置
	unsigned char n;	//下一步可能行走的位置
	unsigned char m;	//马腿、象眼位置
	int SideTag;		//走棋方，经方16，黑方32
	int OverFlag;		//炮的翻山标志
	move * mvArray = MoveArray;

	SideTag = 16 + 16 * side;
	
	p = piece[SideTag];	//将的位置
	if(!p)
		return 0;

	//将的走法
	for(k=0; k<4; k++)//4个方向
	{
		n = p + KingDir[k];	//n为新的可能走到的位置
		if(LegalPosition[side][n] & PositionMask[0])	//将对应下标为0
		{
			if( !(board[n] & SideTag))	//目标位置上没有本方棋子
				if(SaveMove(p, n, mvArray))
					mvArray++;
		}
	}

	//士的走法
	for(i=1; i<=2; i++)
	{
		p = piece[SideTag + i];
		if(!p)
			continue;
		for(k=0; k<4; k++)//4个方向
		{
			n = p + AdvisorDir[k];	//n为新的可能走到的位置
			if(LegalPosition[side][n] & PositionMask[1])	//士将对应下标为1
			{
				if( !(board[n] & SideTag))	//目标位置上没有本方棋子
					if(SaveMove(p, n, mvArray))
						mvArray++;
			}
		}
	}

	//象的走法
	for(i=3; i<=4; i++)
	{
		p = piece[SideTag + i];
		if(!p)
			continue;
		for(k=0; k<4; k++)//4个方向
		{
			n = p + BishopDir[k];	//n为新的可能走到的位置
			if(LegalPosition[side][n] & PositionMask[2])	//象将对应下标为2
			{
				m = p + BishopCheck[k];
				if(!board[m])	//象眼位置无棋子占据
				{
					if( !(board[n] & SideTag))	//目标位置上没有本方棋子
						if(SaveMove(p, n, mvArray))
							mvArray++;
				}
			}
		}
	}
	
	//马的走法
	for(i=5; i<=6; i++)
	{
		p = piece[SideTag + i];
		if(!p)
			continue;
		for(k=0; k<8; k++)//8个方向
		{
			n = p + KnightDir[k];	//n为新的可能走到的位置
			if(LegalPosition[side][n] & PositionMask[3])	//马将对应下标为3
			{
				m = p + KnightCheck[k];
				if(!board[m])	//马腿位置无棋子占据
				{
					if( !(board[n] & SideTag))	//目标位置上没有本方棋子
						if(SaveMove(p, n, mvArray))
							mvArray++;
				}
			}
		}
	}

	//车的走法
	for(i=7; i<=8; i++)
	{
		p = piece[SideTag + i];
		if(!p)
			continue;
		for(k=0; k<4; k++)	//4个方向
		{
			for(j=1; j<10; j++)	//横的最多有8个可能走的位置，纵向最多有9个位置
			{
				n = p + j * RookDir[k];
				if(!(LegalPosition[side][n] & PositionMask[4]))	//车士将对应下标为4
					break;//不合理的位置
				if(! board[n] )	//目标位置上无子
				{
					if(SaveMove(p, n, mvArray))
						mvArray++;
				}
				else if ( board[n] & SideTag)	//目标位置上有本方棋子
					break;
				else	//目标位置上有对方棋子
				{
					if(SaveMove(p, n, mvArray))
						mvArray++;
					break;
				}
			}
		}
	}

	//炮的走法
	for(i=9; i<=10; i++)
	{
		p = piece[SideTag + i];
		if(!p)
			continue;
		for(k=0; k<4; k++)	//4个方向
		{
			OverFlag = 0;
			for(j=1; j<10; j++)	//横的最多有8个可能走的位置，纵向最多有9个位置
			{
				n = p + j * CannonDir[k];
				if(!(LegalPosition[side][n] & PositionMask[5]))	//炮士将对应下标为5
					break;//不合理的位置
				if(! board[n] )	//目标位置上无子
				{
					if(!OverFlag)	//未翻山
						if(SaveMove(p, n, mvArray))
							mvArray++;
					//已翻山则不作处理，自动考察向下一个位置
				}
				else//目标位置上有子
				{
					if (!OverFlag)	//未翻山则置翻山标志
						OverFlag = 1;
					else	//已翻山
					{
						if(! (board[n] & SideTag))//对方棋子
							if(SaveMove(p, n, mvArray))
								mvArray++;
						break;	//不论吃不吃子，都退出此方向搜索
					}
				}
			}
		}	
	}

	//兵的走法
	for(i=11; i<=15; i++)
	{
		p = piece[SideTag + i];
		if(!p)
			continue;
		for(k=0; k<3; k++)//3个方向
		{
			n = p + PawnDir[side][k];	//n为新的可能走到的位置
			if(LegalPosition[side][n] & PositionMask[6])	//兵士将对应下标为6
			{
				if( !(board[n] & SideTag))	//目标位置上没有本方棋子
					if(SaveMove(p, n, mvArray))
						mvArray++;
			}
		}
	}
	return mvArray - MoveArray;
}

int CBoard::LegalMove(move mv)	//判断走法是否合理
{
	move mvArray[128];
	int num = GenAllMove(mvArray);

	int i;
	for(i = 0; i < num; i++)
	{
		if((mv.from == mvArray[i].from) && (mv.to == mvArray[i].to))
		{
			return 1;
		}
	}
	return 0;
}

//判断当前局面是否有合理走法，没有则判输
int CBoard::HasLegalMove()
{
	move mvArray[128];
	int num = GenAllMove(mvArray);;
	return num;
}
