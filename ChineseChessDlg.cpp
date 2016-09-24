#include "stdafx.h"
#include "ChineseChess.h"
#include "ChineseChessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BORDERWIDTH	 13	//棋盘(左右)边缘的宽度
#define BORDERHEIGHT 15	//棋盘(上下)边缘的高度
#define GRILLEWIDTH  35 //棋盘上每个格子的高度
#define GRILLEHEIGHT 35 //棋盘上每个格子的宽度

CChineseChessDlg::CChineseChessDlg(CWnd* pParent /*=NULL*/)
:CDialog(CChineseChessDlg::IDD, pParent)
,m_pobjDc(NULL)
,m_pChessBoardBits(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_SelectMoveFrom = NOMOVE;
	m_SelectMoveTo = NOMOVE;
}

CChineseChessDlg::~CChineseChessDlg()
{
	m_objMemDc.SelectObject(m_pobjOldBmp);
	m_objMemDc.DeleteDC();

	m_pobjDc->DeleteDC();
	m_pobjDc = NULL;

	int sum = m_objChessman.GetImageCount();
	for(int i = 0; i < sum; i++)
	{
		m_objChessman.Remove(0);
	}

	if (m_pChessBoardBits != NULL)
	{
		delete m_pChessBoardBits;
	}
}

void CChineseChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChineseChessDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChineseChessDlg, CDialog)
	//{{AFX_MSG_MAP(CChineseChessDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDM_LET_COMPUTERTHINK, OnLetComputerThink)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CChineseChessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	BITMAP BitMap;
	m_pobjDc = new CClientDC(this);
	m_objMemDc.CreateCompatibleDC(m_pobjDc);
	CBitmap m_BoardBmp;		//棋盘位图
	m_BoardBmp.LoadBitmap(IDB_CHESSBOARD);
	m_BoardBmp.GetBitmap(&BitMap);
	m_nBoardWidth  = BitMap.bmWidth;
	m_nBoardHeight = BitMap.bmHeight;

	m_pChessBoardBits = new unsigned char[m_nBoardWidth * m_nBoardHeight * 4];
	if (m_pChessBoardBits == NULL)
	{
		return FALSE;
	}

	m_BoardBmp.GetBitmapBits(m_nBoardWidth * m_nBoardHeight * 4, m_pChessBoardBits);
	m_BoardBmp.DeleteObject();

	m_objBoardMemBmp.CreateCompatibleBitmap(m_pobjDc, m_nBoardWidth, m_nBoardHeight);
	m_pobjOldBmp = m_objMemDc.SelectObject(&m_objBoardMemBmp);

	m_objChessman.Create(IDB_CHESSMAN, 31, 15, RGB(0,128,128));

	m_objRectBoard.left = BORDERWIDTH;
	m_objRectBoard.right = BORDERWIDTH + GRILLEWIDTH * 9;
	m_objRectBoard.top = BORDERHEIGHT;
	m_objRectBoard.bottom = BORDERHEIGHT + GRILLEHEIGHT * 10;

	InitData();
	beginGame();

	return TRUE;
}

void CChineseChessDlg::OnPaint() 
{
	POINT pt;
	int z;

	m_objBoardMemBmp.SetBitmapBits(m_nBoardWidth * m_nBoardHeight * 4, m_pChessBoardBits);

	for (short i = 0; i < 90; i++)
	{
		if (m_interface[i] == 0)
		{
			if(i == m_SelectMoveFrom)
			{
				pt.x = (i % 9) * GRILLEHEIGHT + BORDERWIDTH ;
				pt.y = (i / 9) * GRILLEWIDTH + BORDERHEIGHT;
				m_objChessman.Draw(&m_objMemDc, 14, pt, ILD_TRANSPARENT);
			}
			continue;
		}
		pt.x = (i % 9) * GRILLEHEIGHT + BORDERWIDTH;
		pt.y = (i / 9) * GRILLEWIDTH + BORDERHEIGHT;
		
		z = IntToSubscript(m_interface[i]);
		
		m_objChessman.Draw(&m_objMemDc, z, pt, ILD_TRANSPARENT);
		if(i == m_SelectMoveFrom)
		{
			m_objChessman.Draw(&m_objMemDc, 14, pt, ILD_TRANSPARENT);
		}
		if(i == m_SelectMoveTo)
		{
			m_objChessman.Draw(&m_objMemDc, 14, pt, ILD_TRANSPARENT);
		}
	}

	if (m_pobjDc != NULL)
	{
		m_pobjDc->BitBlt(0, 0, m_nBoardWidth, m_nBoardHeight, &m_objMemDc, 0, 0, SRCCOPY);
	}
}

void CChineseChessDlg::beginGame() 
{
	InitData();
	m_SelectMoveFrom = NOMOVE;
	m_SelectMoveTo = NOMOVE;
	InvalidateRect(&m_objRectBoard, false);
	UpdateWindow();
	
	m_objBoard.ClearBoard();
	m_objBoard.StringToArray("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w");

	m_gameState = REDTHINKING;
}

BOOL CChineseChessDlg::IsPtInBoard(CPoint point)
{
	return m_objRectBoard.PtInRect(point);
}

void CChineseChessDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!IsPtInBoard(point) || m_gameState != REDTHINKING)
	{
		return;
	}

	int SideTag = 16 + m_HumanSide * 16;

	short dest;
	short from;
	
	//清空高亮显示
	from = m_SelectMoveFrom;
	dest = m_SelectMoveTo;
	m_SelectMoveTo = NOMOVE;
	m_SelectMoveFrom = NOMOVE;
	if(from != NOMOVE)
	{
		RequireDrawCell(from);
	}
	if(dest != NOMOVE)
	{
		RequireDrawCell(dest);
	}

	dest = GetPiecePos(point);

	BYTE piece = m_interface[dest];

	//选手选中本方棋子
	if (piece & SideTag) 
	{
		if (from != NOMOVE)
		{
			//开始已经选中有本方其他棋子,则取消高亮显示原来选中的棋子
			m_SelectMoveFrom = NOMOVE;
			RequireDrawCell(from);
		}
		m_SelectMoveFrom = dest;	//高亮显示新选中的棋子
		RequireDrawCell(dest);
	}
	else
	{
		//棋子落在空处或者他方的棋子上
		if (from != NOMOVE)	//如果已经先选中本方棋子
		{
			//将10*9的棋盘位置转换成16*16的棋盘位置
			move mv;
			mv.from = ((from / 9 + 3) * 16 + from % 9 + 3);
			mv.to = ((dest / 9 + 3) * 16 + dest % 9 + 3);

			//走法合理性检验，源位置z，目的位置k
			if (m_objBoard.LegalMove(mv)) 
			{
				m_objBoard.MakeMove(mv);
				m_interface[dest] = m_interface[from];
				m_interface[from] = 0;

				m_SelectMoveTo = dest;
				RequireDrawCell(dest);
				m_SelectMoveFrom = from;	//着重显示走法起始点
				RequireDrawCell(from);		//将源点及目的点重新显示

				//判断胜负
				bool bIsHasLegalMove = m_objBoard.HasLegalMove();
				if (!bIsHasLegalMove)
				{
					m_gameState = GAMEOVER;
					MessageBox("恭喜你赢了", "系统消息");
					return;
				}

				m_gameState = BLACKTHINKING;

				PostMessage(WM_COMMAND, IDM_LET_COMPUTERTHINK);
			}
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

short CChineseChessDlg::GetPiecePos(POINT pt)
{
	if (!m_objRectBoard.PtInRect(pt))
	{
		return -1;
	}

	short x = (pt.x-m_objRectBoard.left) / GRILLEWIDTH;
	short y = (pt.y-m_objRectBoard.top)  / GRILLEHEIGHT;

	return x + y * 9;
}

void CChineseChessDlg::RequireDrawCell(short pos)
{
	CRect rect = GetPieceRect(pos);
	InvalidateRect(&rect, false);
	UpdateWindow();
}

CRect CChineseChessDlg::GetPieceRect(short pos)
{
	short x = BORDERWIDTH + (pos % 9) * GRILLEWIDTH;
	short y = BORDERHEIGHT + (pos / 9) * GRILLEHEIGHT;
	CRect rect(x, y, x + GRILLEWIDTH, y + GRILLEHEIGHT);
	return rect;
}

void CChineseChessDlg::OnLetComputerThink()
{
	if (m_gameState == GAMEOVER)
	{
		return;
	}

	m_objBoard.ComputerThink();

	short z = m_objBoard.BestMove.from;
	short k = m_objBoard.BestMove.to;

	if(z == 0)
	{
		m_gameState = GAMEOVER;
		MessageBox("恭喜你赢了", "系统消息");
		return;
	}

	m_objBoard.MakeMove(m_objBoard.BestMove);

	//清空高亮显示
	short zz = m_SelectMoveFrom;
	short kk = m_SelectMoveTo;
	m_SelectMoveFrom = NOMOVE;
	m_SelectMoveTo = NOMOVE;
	RequireDrawCell(zz); 
	RequireDrawCell(kk);

	zz = ((z / 16 -3) * 9 + z % 16 -3);	//将16*16的棋盘位置转换成10*9的棋盘位置
	kk = ((k / 16 -3) * 9 + k % 16 -3);

	m_interface[kk] = m_interface[zz];
	m_interface[zz] = 0;

	//高亮显示电脑走法
	m_SelectMoveFrom = zz;
	m_SelectMoveTo = kk;
	RequireDrawCell(zz); 
	RequireDrawCell(kk);

	bool bIsHasLegalMove = m_objBoard.HasLegalMove();
	if (!bIsHasLegalMove)
	{
		m_gameState = GAMEOVER;
		MessageBox("你输了", "系统消息");
		return;
	}

	m_gameState = REDTHINKING;
}

void CChineseChessDlg::InitData()
{
	static const unsigned char board[BOARD_SIZE] = 
	{
		39, 37, 35, 33, 32, 34, 36, 38, 40,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 41, 0, 0, 0, 0, 0, 42, 0,
		43, 0, 44, 0, 45, 0, 46, 0, 47,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		27, 0, 28, 0, 29, 0, 30, 0, 31,
		0, 25, 0, 0, 0, 0, 0, 26, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		23, 21, 19, 17, 16, 18, 20, 22, 24
	};

	for (int i = 0; i < BOARD_SIZE; i++) 
	{
		m_interface[i] = board[i];
	}

	m_HumanSide = 0;
	m_gameState = GAMEOVER;
}

int CChineseChessDlg::IntToSubscript(int a)
{
	if(a<16 && a>=48)
	{
		return 14;
	}

	switch(a)
	{
		//红方棋子
		case 16:	return 0;
		case 17:
		case 18:	return 1;
		case 19:
		case 20:	return 2;
		case 21:
		case 22:	return 3;
		case 23:
		case 24:	return 4;
		case 25:
		case 26:	return 5;
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:	return 6;

		//黑方棋子
		case 32:	return 7;
		case 33:
		case 34:	return 8;
		case 35:
		case 36:	return 9;
		case 37:
		case 38:	return 10;
		case 39:
		case 40:	return 11;
		case 41:
		case 42:	return 12;
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:	return 13;

		default:	return 14;
	}
}
