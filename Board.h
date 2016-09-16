#if !defined(AFX_BOARD_H__F6F321A6_8261_4448_A440_7B50D85B1BAB__INCLUDED_)
#define AFX_BOARD_H__F6F321A6_8261_4448_A440_7B50D85B1BAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "def.h"
const int MaxValue = 3000;	//估值最大值

class CBoard  
{
public:
	CBoard();
	virtual ~CBoard();

public:
	int side;					// 轮到哪方走，0表示红方，1表示黑方
	unsigned char board[256];	// 棋盘数组
	unsigned char piece[48];	// 棋子数组
	char FenString[128];		// 局面的FEN串格式

	move MoveStack[128];		// 执行的走法栈
	int StackTop;				// 栈顶指针,指向栈顶元素的下一位置,=0表示栈空
	move BestMove;				// 搜索得到的最佳走法
	int ply;					// 当前搜索深度
	int MaxDepth;				// 最大搜索深度

	// 局面表示相关函数
	void ClearBoard();						// 清空棋盘数组
	char IntToChar(int a);					// 棋子整数值转换成字符值
	int CharToSubscript(char ch);			// FEN串中棋子对应的数组下标,下标0，1，2，3，4，5，6分别对应表示将，仕，象，马，车，炮，兵

	void AddPiece(int pos, int pc);			// 在pos位置增加棋子pc
	void StringToArray(const char *FenStr); // 将FEN串表示的局面转换成一维数组

	// 走法生成相关函数
	int SaveMove(unsigned char from, unsigned char to,move * mv);//保存生成的走法,成功返回1，失败返回0
	int GenAllMove(move * MoveArray);	//生成所有的走法
	int Check(int lSide);				//检测lSide一方是否被将军，是被将军返回1，否则返回0
	int LegalMove(move mv);				//判断走法是否合理
	int HasLegalMove();					//判断当前局面是否有合理走法，没有则判输

	// 评估函数
	short Eval();						// 评估函数
	int IntToSubscript(int a);			// 棋子整数值转换成下标,下标0，1，2，3，4，5，6分别对应表示将，仕，象，马，车，炮，兵

	// 搜索函数
	void ComputerThink();	//电脑思考
	int AlphaBetaSearch(int depth, int alpha, int beta);	// Alpha-Beta搜索算法
	bool MakeMove(move m);	// 执行走法
	void UnMakeMove();		// 撤消走法
	void ChangeSide();		// 交换走棋方
};
#endif
