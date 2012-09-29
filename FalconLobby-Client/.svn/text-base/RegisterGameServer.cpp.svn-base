// RegisterGameServer.cpp : implementation file
//

#include "stdafx.h"
#include "FalconLobby.h"
#include "RegisterGameServer.h"
#include ".\registergameserver.h"
#include "const.h"

#include <string>
#include <sstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRegisterGameServer dialog

IMPLEMENT_DYNAMIC(CRegisterGameServer, CDialog)
CRegisterGameServer::CRegisterGameServer(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterGameServer::IDD, pParent)
{
}

CRegisterGameServer::~CRegisterGameServer()
{
}

void CRegisterGameServer::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_NAME, m_ctlName);
}


BEGIN_MESSAGE_MAP(CRegisterGameServer, CDialog)
   ON_BN_CLICKED(IDC_REGISTER, OnBnClickedRegister)
END_MESSAGE_MAP()


// CRegisterGameServer message handlers

BOOL CRegisterGameServer::OnInitDialog()
{
   CDialog::OnInitDialog();

   m_ctlName.SetLimitText(MAXLEN_GAMENAME);
   m_ctlName.SetFocus();

   CenterWindow();

   m_client.setWnd(GetParent()->GetSafeHwnd());

   return FALSE;
   //return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegisterGameServer::OnBnClickedRegister()
{
   CString s;
   m_ctlName.GetWindowText(s);

   string error;
   string name = s;
   string token;
   if (m_client.registerNewGame(name,token,error))
   {
      AfxMessageBox("Registration Succeeded!");

      setSettingsString("gameserver","name",name);
      setSettingsString("gameserver","token",token);

      EndDialog(IDOK);
   }
   else
   {
      stringstream str;
      str << "Registration Failed: " << error;
      AfxMessageBox(str.str().c_str());
   }
}
