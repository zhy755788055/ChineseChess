// duanDlg.h : header file
//

#if !defined(AFX_DUANDLG_H__BC9E6178_3857_4B19_A937_D0B32A100772__INCLUDED_)
#define AFX_DUANDLG_H__BC9E6178_3857_4B19_A937_D0B32A100772__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "def.h"
#include "Board.h"

#define	SIZE_X			9
#define	SIZE_Y			10
#define BOARD_SIZE		SIZE_X*SIZE_Y
#define NOMOVE				-1
#define EMPTY	7
#define BLACK	1
#define RED	0
//m_gameState的三种状态
#define REDTHINKING		0
#define BLACKTHINKING	1
#define GAMEOVER			2
//自定义消息
#define IDM_LET_COMPUTERTHINK WM_USER +100

/////////////////////////////////////////////////////////////////////////////
// CDuanDlg dialog

class CDuanDlg : public CDialog
{
// Construction
public:
	int IntToSubscript(int a);//棋子整数值转换成下标表示，用于界面显示
				//返回值：0：红方获胜，1黑方获胜，-1未分胜负
	void InitData();		//初始化数据
	CRect GetPieceRect(short pos);		//棋盘位置对应的矩形区域
	void RequireDrawCell(short pos);	//重绘棋盘上的一点
	short GetPiecePos(POINT pt);	//鼠标点对应的棋盘位置
	BOOL IsPtInBoard(CPoint point);	//鼠标点是否在棋盘内
	CDuanDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDuanDlg)
	enum { IDD = IDD_DUAN_DIALOG };
	CStatic	m_RedTimePass_Ctr;
	CStatic	m_RedTimeLeft_Ctr;
	CStatic	m_BlkTimePass_Ctr;
	CStatic	m_BlkTimeLeft_Ctr;
	CButton	m_ButExit;
	CButton	m_ButBegin;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDuanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDuanDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButBegin();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLetComputerThink();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_BoardBmp;		//棋盘位图
	CImageList m_Chessman;	//棋子图列表
	int m_nBoardWidth, m_nBoardHeight;//棋盘位图的宽高


	//时间控制相关的变量
	CTimeSpan m_tsBlkTimeLeft,m_tsRedTimeLeft;
	CTimeSpan m_tsBlkTimePass,m_tsRedTimePass;
	CTimeSpan m_TotalTime;
	UINT m_BlkTimer,m_RedTimer;

	CBoard m_Board;

	//界面显示控制变量
	CRect	rectBoard;		//棋盘矩形框
	short	m_SelectMoveFrom,m_SelectMoveTo;	//一个走法的起始点，用于高亮显示
	short	m_ComputerSide, m_HumanSide;
	BYTE	m_interface[BOARD_SIZE];//棋盘数组，保存的棋子
	short	m_gameState;	//棋局状态，结束/红方/黑方

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUANDLG_H__BC9E6178_3857_4B19_A937_D0B32A100772__INCLUDED_)
