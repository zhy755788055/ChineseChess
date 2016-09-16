// duan.h : main header file for the DUAN application
//

#if !defined(AFX_DUAN_H__0714199D_D7D3_4AE6_A52F_03431F096ACF__INCLUDED_)
#define AFX_DUAN_H__0714199D_D7D3_4AE6_A52F_03431F096ACF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDuanApp:
// See duan.cpp for the implementation of this class
//

class CDuanApp : public CWinApp
{
public:
	CDuanApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDuanApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDuanApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUAN_H__0714199D_D7D3_4AE6_A52F_03431F096ACF__INCLUDED_)
