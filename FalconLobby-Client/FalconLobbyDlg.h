// FalconLobbyDlg.h : header file
//

#pragma once

#include "Settings.h"
#include "Client.h"
#include "ServerDlg.h"
#include "Falcon.h"

// CFalconLobbyDlg dialog
class CFalconLobbyDlg : public CDialog, public Settings
{
// Construction
public:
	CFalconLobbyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FALCONLOBBY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

   // popup menu
   CMenu m_popupMenuParent;
   CMenu * m_popupMenu;

   // server info dialog
   CServerDlg * m_serverDlg;

   // client
   Client m_client;

   // falcon interface class
   Falcon m_falcon;

   // time of last auto-download
   DWORD m_lastAutoDownloadTime;

   // time of last auto-upload
   DWORD m_lastAutoUploadTime;

   void OnTrayViewActiveGameServers();
   void OnTrayManualRetrieve();
   void OnTrayAutoRetrieve();
   void OnTrayPublishMyGameServer();
   void OnTrayEditSettingsFile();
   void OnTrayFalconHomePage();
   void OnTrayAbout();
   void OnTrayHelp();
   void OnTrayExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL DestroyWindow();
protected:
   virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
//   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
   afx_msg void OnTimer(UINT nIDEvent);
};
