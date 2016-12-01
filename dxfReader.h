// dxfReader.h : main header file for the DXFREADER application
//

#if !defined(AFX_DXFREADER_H__BA480E95_458C_4B85_98BB_A7878D5C4F24__INCLUDED_)
#define AFX_DXFREADER_H__BA480E95_458C_4B85_98BB_A7878D5C4F24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDxfReaderApp:
// See dxfReader.cpp for the implementation of this class
//

class CDxfReaderApp : public CWinApp
{
public:
	CDxfReaderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDxfReaderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDxfReaderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXFREADER_H__BA480E95_458C_4B85_98BB_A7878D5C4F24__INCLUDED_)
