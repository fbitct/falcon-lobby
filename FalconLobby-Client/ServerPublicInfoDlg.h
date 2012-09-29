#pragma once
#include "afxwin.h"

#include <string>

// CServerPublicInfoDlg dialog

class CServerPublicInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerPublicInfoDlg)

public:
	CServerPublicInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServerPublicInfoDlg();

   std::string m_desc;
   std::string m_homepage;
   std::string m_voicecomm;

// Dialog Data
	enum { IDD = IDD_SERVER_PUBLICINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   CEdit m_ctlDesc;
   CEdit m_ctlHomePage;
   CEdit m_ctlVoiceComm;
   afx_msg void OnBnClickedOk();
   virtual BOOL OnInitDialog();
};
