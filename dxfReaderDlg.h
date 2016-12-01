// dxfReaderDlg.h : header file
//

#if !defined(AFX_DXFREADERDLG_H__461D7E87_5002_4F51_A87E_C9687A3B2816__INCLUDED_)
#define AFX_DXFREADERDLG_H__461D7E87_5002_4F51_A87E_C9687A3B2816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDxfReaderDlg dialog

class CDxfReaderDlg : public CDialog
{
// Construction
public:
	CDxfReaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDxfReaderDlg)
	enum { IDD = IDD_DXFREADER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDxfReaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDxfReaderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton3();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXFREADERDLG_H__461D7E87_5002_4F51_A87E_C9687A3B2816__INCLUDED_)
