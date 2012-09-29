#pragma once
#include "afxwin.h"

#include "Settings.h"
#include "Client.h"

// CRegisterGameServer dialog

class CRegisterGameServer : public CDialog, public Settings
{
	DECLARE_DYNAMIC(CRegisterGameServer)

public:
	CRegisterGameServer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegisterGameServer();

// Dialog Data
	enum { IDD = IDD_REGISTERGAMESERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   Client m_client;

	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
   CEdit m_ctlName;
   afx_msg void OnBnClickedRegister();
};
