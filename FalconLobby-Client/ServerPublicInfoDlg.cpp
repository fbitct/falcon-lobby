// ServerPublicInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FalconLobby.h"
#include "ServerPublicInfoDlg.h"
#include ".\serverpublicinfodlg.h"
#include "const.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerPublicInfoDlg dialog

IMPLEMENT_DYNAMIC(CServerPublicInfoDlg, CDialog)
CServerPublicInfoDlg::CServerPublicInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerPublicInfoDlg::IDD, pParent)
{
}

CServerPublicInfoDlg::~CServerPublicInfoDlg()
{
}

void CServerPublicInfoDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_DESC, m_ctlDesc);
   DDX_Control(pDX, IDC_HOMEPAGE, m_ctlHomePage);
   DDX_Control(pDX, IDC_VOICECOMM, m_ctlVoiceComm);
}


BEGIN_MESSAGE_MAP(CServerPublicInfoDlg, CDialog)
   ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
END_MESSAGE_MAP()


// CServerPublicInfoDlg message handlers

void CServerPublicInfoDlg::OnBnClickedOk()
{
   CString s;
   m_ctlDesc.GetWindowText(s);
   m_desc = s;
   m_ctlHomePage.GetWindowText(s);
   m_homepage = s;
   m_ctlVoiceComm.GetWindowText(s);
   m_voicecomm = s;

   EndDialog(IDOK);
}

BOOL CServerPublicInfoDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   m_ctlDesc.SetLimitText(MAXLEN_GAMEDESC);
   m_ctlHomePage.SetLimitText(MAXLEN_GAMEHOMEPAGE);
   m_ctlVoiceComm.SetLimitText(MAXLEN_GAMEVOICECOMMS);

   CenterWindow();

   m_ctlDesc.SetFocus();

   return FALSE;
   //return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
