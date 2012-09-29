// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FalconLobby.h"
#include "ServerDlg.h"
#include ".\serverdlg.h"
#include "const.h"
#include "ServerPublicInfoDlg.h"
#include "RegisterGameServer.h"

#include <sstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerDlg dialog

IMPLEMENT_DYNAMIC(CServerDlg, CDialog)
CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
   Create(IDD_SERVER,pParent);
}

CServerDlg::~CServerDlg()
{
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_NAME, m_ctlName);
   DDX_Control(pDX, IDC_TOKEN, m_ctlToken);
   DDX_Control(pDX, IDC_AUTOPUB, m_ctlAutoPub);
   DDX_Control(pDX, IDC_BACKUPWARNING, m_ctlWarning);
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
   ON_WM_CLOSE()
   ON_WM_TIMER()
   ON_BN_CLICKED(IDC_AUTOPUB, OnBnClickedAutopub)
   ON_WM_CTLCOLOR()
   ON_BN_CLICKED(IDC_SETPUBLICINFO, OnBnClickedSetpublicinfo)
   ON_BN_CLICKED(IDC_REGISTERNEWGAMESERVER, OnBnClickedRegisternewgameserver)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   LOGFONT lf;
   GetFont()->GetLogFont(&lf);
   lf.lfWeight = FW_BOLD;
   m_boldFont.CreateFontIndirect(&lf);
   m_ctlName.SetFont(&m_boldFont);
   m_ctlToken.SetFont(&m_boldFont);
   GetFont()->GetLogFont(&lf);
   lf.lfWeight = FW_BOLD;
   lf.lfItalic = TRUE;
   m_warningFont.CreateFontIndirect(&lf);
   m_ctlWarning.SetFont(&m_warningFont);

   CenterWindow();

   m_client.setWnd(GetParent()->GetSafeHwnd());

   update(true);

   m_timer = SetTimer(SERVERDLG_UPDATE_TIMER_ID,SERVERDLG_UPDATE_DELAY_MS,NULL);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerDlg::OnClose()
{
   KillTimer(m_timer);

   __super::OnClose();
}

void CServerDlg::update(bool forceUpdate /*= false*/)
{
   string newName = getSettingsString("gameserver","name");
   string newToken = getSettingsString("gameserver","token");
   bool newAutoPub = (getSettingsInt("options","auto_publish_gameserver") != 0);

   if ((newName != m_name) || forceUpdate)
   {
      m_name = newName;
      m_ctlName.SetWindowText(m_name.c_str());
   }

   if ((newToken != m_token) || forceUpdate)
   {
      m_token = newToken;
      m_ctlToken.SetWindowText(m_token.c_str());
   }

   if ((newAutoPub != m_autopub) || forceUpdate)
   {
      m_autopub = newAutoPub;
      m_ctlAutoPub.SetCheck(m_autopub ? BST_CHECKED : BST_UNCHECKED);
   }
}

void CServerDlg::OnTimer(UINT nIDEvent)
{
   if (nIDEvent == SERVERDLG_UPDATE_TIMER_ID)
   {
      update();
   }

   __super::OnTimer(nIDEvent);
}

void CServerDlg::OnBnClickedAutopub()
{
   setSettingsInt("options","auto_publish_gameserver",    
      ((m_ctlAutoPub.GetCheck() == BST_CHECKED) ? 1 : 0));
}

HBRUSH CServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
   HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

   if (pWnd->GetDlgCtrlID() == IDC_BACKUPWARNING)
   {
      pDC->SetTextColor(BACKUPWARNING_COLOR);
   }

   return hbr;
}

void CServerDlg::OnBnClickedSetpublicinfo()
{
   CServerPublicInfoDlg dlg;
   if (dlg.DoModal() == IDOK)
   {
      string error;
      if (m_client.setGameInfo(getSettingsString("gameserver","token"),dlg.m_homepage,dlg.m_voicecomm,dlg.m_desc,error))
      {
         AfxMessageBox("Set Game Server Public Info Successfully");
      }
      else
      {
         stringstream str;
         str << "Failed to set Game Server Public Info: " << error;
         AfxMessageBox(str.str().c_str());
      }
   }
}

void CServerDlg::OnBnClickedRegisternewgameserver()
{
   if (AfxMessageBox("Registering a new Game Server will cause the existing Name and Token to be "
                     "overwritten in the settings file with the new values.  If you'd like to later "
                     "use the existing registered server information, you should backup the Name "
                     "and Token before continuing.\n\n"
                     "Are you sure you want to continue and Register a New Game Server?",
                     MB_YESNO|MB_ICONQUESTION) == IDYES)
   {
      CRegisterGameServer dlg(GetParent());
      dlg.DoModal();
   }
}
