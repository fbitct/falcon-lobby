#pragma once
#include "afxwin.h"
#include "Settings.h"
#include "Client.h"

#include <string>

// CServerDlg dialog

class CServerDlg : public CDialog, public Settings
{
	DECLARE_DYNAMIC(CServerDlg)

public:
	CServerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServerDlg();

// Dialog Data
	enum { IDD = IDD_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   CFont m_boldFont;
   CFont m_warningFont;

   void update(bool forceUpdate = false);

   std::string m_name;
   std::string m_token;
   bool m_autopub;

   UINT_PTR m_timer;

   Client m_client;

	DECLARE_MESSAGE_MAP()
public:
   CStatic m_ctlName;
   CStatic m_ctlToken;
   virtual BOOL OnInitDialog();
   afx_msg void OnClose();
   afx_msg void OnTimer(UINT nIDEvent);
   afx_msg void OnBnClickedAutopub();
   CButton m_ctlAutoPub;
   CStatic m_ctlWarning;
   afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
   afx_msg void OnBnClickedSetpublicinfo();
   afx_msg void OnBnClickedRegisternewgameserver();
};
