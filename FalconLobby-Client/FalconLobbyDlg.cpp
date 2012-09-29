autoretrieve_active_servers// FalconLobbyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FalconLobby.h"
#include "FalconLobbyDlg.h"
#include "PhoneBook.h"
#include "const.h"
#include "RegisterGameServer.h"
#include "INIFile.h"

#include <sstream>
#include <map>
#include ".\falconlobbydlg.h"
#include "afxwin.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
   CAboutDlg();

// Dialog Data
   enum { IDD = IDD_ABOUTBOX };

   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
   DECLARE_MESSAGE_MAP()

   CFont m_boldFont;

   bool m_iconToggle;

   map<bool,HICON> icons;

public:
   CStatic m_ctlTitle;
   CStatic m_ctlCredits;
   virtual BOOL OnInitDialog();
   CStatic m_ctlIcon;
//   afx_msg void OnTimer(UINT nIDEvent);
   afx_msg void OnTimer(UINT nIDEvent);
};

BOOL CAboutDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   {
      stringstream str;
      str << PRODUCT_NAME << " " << PRODUCT_VERSION << " (" << PRODUCT_DATE << ")";
      m_ctlTitle.SetWindowText(str.str().c_str());
   }

   {
      stringstream str;
      str << PRODUCT_CREDITS;
      m_ctlCredits.SetWindowText(str.str().c_str());
   }

   LOGFONT lf;
   GetFont()->GetLogFont(&lf);
   lf.lfWeight = FW_BOLD;
   m_boldFont.CreateFontIndirect(&lf);
   m_ctlTitle.SetFont(&m_boldFont);

   icons[false] = AfxGetApp()->LoadIcon(IDI_TRAY_ICON);
   icons[true] = AfxGetApp()->LoadIcon(IDI_TRAY_ICON_LIT);

   SetTimer(ABOUTBOX_ICON_FLASH_TIMER_ID,ABOUTBOX_ICON_FLASHING_DELAY_MS,NULL);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD), m_iconToggle(false)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_TITLE, m_ctlTitle);
   DDX_Control(pDX, IDC_CREDITS, m_ctlCredits);
   DDX_Control(pDX, IDC_ABICON, m_ctlIcon);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
   ON_WM_TIMER()
END_MESSAGE_MAP()

void CAboutDlg::OnTimer(UINT nIDEvent)
{
   // flash the about box icon
   if (nIDEvent == ABOUTBOX_ICON_FLASH_TIMER_ID)
   {
      m_iconToggle = !m_iconToggle;
      m_ctlIcon.SetIcon(icons[m_iconToggle]);
   }

   CDialog::OnTimer(nIDEvent);
}


// CFalconLobbyDlg dialog



CFalconLobbyDlg::CFalconLobbyDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CFalconLobbyDlg::IDD, pParent),
     m_serverDlg(NULL),
     m_lastAutoDownloadTime(0),
     m_lastAutoUploadTime(0)
{
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFalconLobbyDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFalconLobbyDlg, CDialog)
   ON_WM_SYSCOMMAND()
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()
   //}}AFX_MSG_MAP
   ON_WM_TIMER()
END_MESSAGE_MAP()


// CFalconLobbyDlg message handlers

BOOL CFalconLobbyDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   // Add "About..." menu item to system menu.

   // IDM_ABOUTBOX must be in the system command range.
   ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
   ASSERT(IDM_ABOUTBOX < 0xF000);

   CMenu* pSysMenu = GetSystemMenu(FALSE);
   if (pSysMenu != NULL)
   {
      CString strAboutMenu;
      strAboutMenu.LoadString(IDS_ABOUTBOX);
      if (!strAboutMenu.IsEmpty())
      {
         pSysMenu->AppendMenu(MF_SEPARATOR);
         pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
      }
   }

   // setup min int values for settings
   setSettingsMinInt("options","auto_download_period_ms",MIN_DLUL_PERIODS_MS);
   setSettingsMinInt("options","auto_publish_gameserver_period_ms",MIN_DLUL_PERIODS_MS);
   setSettingsMinInt("options","request_timeout_ms",MIN_REQUESTTIMEOUT_MS);

   // Set the icon for this dialog.  The framework does this automatically
   //  when the application's main window is not a dialog
   SetIcon(m_hIcon, TRUE);       // Set big icon
   SetIcon(m_hIcon, FALSE);      // Set small icon

   // set dialog size and pos so its out of sight and prevent
   // a taskbar button from being created by removing the
   // WS_EX_APPWINDOW extended style.  this is done because
   // interaction with the app is via the tray icon
   SetWindowPos(&wndBottom,0,0,0,0,0);
   ModifyStyleEx(WS_EX_APPWINDOW,0);

   // load popup menu
   m_popupMenuParent.LoadMenu(IDR_POPUP_MENU);
   m_popupMenu = m_popupMenuParent.GetSubMenu(0);

   // initialize popup menu state
   m_popupMenu->CheckMenuItem(ID_TRAY_AUTORETRIEVE,MF_BYCOMMAND|
      (getSettingsInt("options","autoretrieve_active_servers") == 1 ? MF_CHECKED : MF_UNCHECKED));

   // add the tray icon
   NOTIFYICONDATA nid;
   memset(&nid,0,sizeof(nid));
   nid.cbSize           = sizeof(nid);
   nid.hWnd             = GetSafeHwnd();
   nid.uID              = TRAY_ICON_ID;
   nid.uCallbackMessage = TRAY_ICON_MSG;
   nid.hIcon            = AfxGetApp()->LoadIcon(IDI_TRAY_ICON);
   nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   {
      stringstream str;
      str << PRODUCT_NAME << " "
          << PRODUCT_VERSION << " ("
          << PRODUCT_DATE << ")";
      strcpy(nid.szTip,str.str().c_str());
   }
   Shell_NotifyIcon(NIM_ADD,&nid);

   // set client's window handle to allow updating tray icon
   // while a request is being processed
   m_client.setWnd(GetSafeHwnd());

   // start timer for auto-download
   SetTimer(AUTO_DOWNLOAD_TIMER_ID,AUTO_DOWNLOAD_LOOP_DELAY_MS,NULL);

   // start timer for auto-upload of game state
   SetTimer(AUTO_UPLOAD_TIMER_ID,AUTO_UPLOAD_LOOP_DELAY_MS,NULL);

   // start timer for UI updates
   SetTimer(UIUPDATE_TIMER_ID,UIUPDATE_LOOP_DELAY_MS,NULL);

   // set the window title
   SetWindowText(MAINWND_TITLE);

   return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFalconLobbyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
   if ((nID & 0xFFF0) == IDM_ABOUTBOX)
   {
      CAboutDlg dlgAbout;
      dlgAbout.DoModal();
   }
   else
   {
      CDialog::OnSysCommand(nID, lParam);
   }
}

BOOL CFalconLobbyDlg::DestroyWindow()
{
   // remove the tray icon
   NOTIFYICONDATA nid;
   memset(&nid,0,sizeof(nid));
   nid.cbSize           = sizeof(nid);
   nid.hWnd             = GetSafeHwnd();
   nid.uID              = TRAY_ICON_ID;
   Shell_NotifyIcon(NIM_DELETE,&nid);

   // delete the server dialog
   if (m_serverDlg != NULL)
   {
      delete m_serverDlg;
      m_serverDlg = NULL;
   }

   return CDialog::DestroyWindow();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFalconLobbyDlg::OnPaint()
{
   if (IsIconic())
   {
      CPaintDC dc(this); // device context for painting

      SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

      // Center icon in client rectangle
      int cxIcon = GetSystemMetrics(SM_CXICON);
      int cyIcon = GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x = (rect.Width() - cxIcon + 1) / 2;
      int y = (rect.Height() - cyIcon + 1) / 2;

      // Draw the icon
      dc.DrawIcon(x, y, m_hIcon);
   }
   else
   {
      CDialog::OnPaint();
   }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFalconLobbyDlg::OnQueryDragIcon()
{
   return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CFalconLobbyDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
   // handle right-click on tray icon
   if ((message == TRAY_ICON_MSG) &&
       (wParam == TRAY_ICON_ID) &&
       (lParam == WM_RBUTTONDOWN))
   {
      // track popup menu at mouse position
      POINT p;
      GetCursorPos(&p);
      SetForegroundWindow();
      m_popupMenu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_BOTTOMALIGN,p.x,p.y,this,0);
   }

   // shutdown message received from invocation of the executable with the -shutdown flag?
   if (message == APP_SHUTDOWN_MSG)
   {
      PostMessage(WM_CLOSE,0,0);
   }

   return CDialog::WindowProc(message, wParam, lParam);
}

void CFalconLobbyDlg::OnTimer(UINT nIDEvent)
{
   // UI update timer
   if (nIDEvent == UIUPDATE_TIMER_ID)
   {
      // ensure the auto-retrieve menu check matches the settings file

      bool isChecked = m_popupMenu->GetMenuState(ID_TRAY_AUTORETRIEVE,MF_BYCOMMAND) && MF_CHECKED;
      bool shouldBeChecked = getSettingsInt("options","autoretrieve_active_servers") != 0;

      if (isChecked != shouldBeChecked)
      {
         m_popupMenu->CheckMenuItem(ID_TRAY_AUTORETRIEVE,MF_BYCOMMAND|
            (shouldBeChecked ? MF_CHECKED : MF_UNCHECKED));
      }
   }

   // auto-download timer
   if (nIDEvent == AUTO_DOWNLOAD_TIMER_ID)
   {
      // auto-download enabled?
      if (getSettingsInt("options","autoretrieve_active_servers") != 0)
      {
         // has sufficient time passed since the last auto-download?
         if ((int)(GetTickCount() - m_lastAutoDownloadTime) > getSettingsInt("options","auto_download_period_ms"))
         {
            // retrieve active games
            vector<Client::Game> activeGames;
            string error;
            if (m_client.getActiveGames(activeGames,error))
            {
               // write out received list of games
               PhoneBook pb;
               vector<Client::Game>::iterator i;
               for (i = activeGames.begin(); i != activeGames.end(); ++i)
               {
                  PhoneBook::Entry entry;
                  entry.game_name = i->game_name;
                  entry.game_ip = i->game_ip;
                  entry.game_theater = i->game_theater;
                  pb.addEntry(entry);
               }
               string err;
               pb.commitChanges(false,(getSettingsInt("options","merge_phonebook_entries") != 0),err);
            }

            m_lastAutoDownloadTime = GetTickCount();
         }
      }
   }

   // auto-upload timer
   if (nIDEvent == AUTO_UPLOAD_TIMER_ID)
   {
      // auto-upload enabled?
      if (getSettingsInt("options","auto_publish_gameserver") != 0)
      {
         // does the settings file contain game server info?
         if ((getSettingsString("gameserver","name").length() != 0) &&
             (getSettingsString("gameserver","token").length() != 0))
         {
            // has sufficient time passed since the last auto-upload?
            if ((int)(GetTickCount() - m_lastAutoUploadTime) > getSettingsInt("options","auto_publish_gameserver_period_ms"))
            {
               // determine server state
               bool serverState = m_falcon.isMPServerRunning();

               // if server is up, get current theatre
               string gameTheater;
               if (serverState)
               {
                  gameTheater = m_falcon.getTheater();
               }

               static bool firstTime = true;
               static bool prevServerState = false;

               if (firstTime ||
                   (serverState != prevServerState) ||
                   serverState)
               {
                  firstTime = false;
                  prevServerState = serverState;

                  TRACE1("reporting state %d to server\n", serverState);
                  // report state to server
                  string err;
                  m_client.setGameState(getSettingsString("gameserver","token"),serverState,gameTheater,err);
               }

               m_lastAutoUploadTime = GetTickCount();
            }
         }
      }
   }

   __super::OnTimer(nIDEvent);
}

BOOL CFalconLobbyDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
   // dispatch selection events from the popup menu to the appropriate handler

   switch(LOWORD(wParam))
   {
   case ID_TRAY_VIEWACTIVEGAMESERVERS:
      OnTrayViewActiveGameServers();
      break;

   case ID_TRAY_MANUALRETRIEVE:
      OnTrayManualRetrieve();
      break;

   case ID_TRAY_AUTORETRIEVE:
      OnTrayAutoRetrieve();
      break;

   case ID_TRAY_PUBLISHMYGAMESERVER:
      OnTrayPublishMyGameServer();
      break;

   case ID_TRAY_EDITSETTINGSFILE:
      OnTrayEditSettingsFile();
      break;

   case ID_TRAY_FALCONLOBBYHOMEPAGE:
      OnTrayFalconHomePage();
      break;

   case ID_TRAY_ABOUT:
      OnTrayAbout();
      break;

   case ID_TRAY_HELP:
      OnTrayHelp();
      break;

   case ID_TRAY_EXIT:
      OnTrayExit();
      break;
   }

   return CDialog::OnCommand(wParam, lParam);
}

void CFalconLobbyDlg::OnTrayViewActiveGameServers()
{
   // build URL string
   stringstream str;
   str << "http://"
       << getSettingsString("gameserver_registry","host")
       << ":" << getSettingsInt("gameserver_registry","port")
       << getSettingsString("gameserver_registry","games_path");

   ShellExecute(NULL,"open",str.str().c_str(),NULL,NULL,SW_SHOW);
}

void CFalconLobbyDlg::OnTrayManualRetrieve()
{
   // retrieve active games
   vector<Client::Game> activeGames;
   string error;
   if (m_client.getActiveGames(activeGames,error))
   {
      // add phone book entries
      PhoneBook pb;
      vector<Client::Game>::iterator i;
      for (i = activeGames.begin(); i != activeGames.end(); ++i)
      {
         PhoneBook::Entry entry;
         entry.game_name = i->game_name;
         entry.game_ip = i->game_ip;
         entry.game_theater = i->game_theater;
         pb.addEntry(entry);
      }

      // commit phone book updates
      string err;
      if (!pb.commitChanges(true,(getSettingsInt("options","merge_phonebook_entries") != 0),err))
      {
         stringstream str;
         str << "Unable to save Active Game List to Falcon\n"
               << err;
         AfxMessageBox(str.str().c_str());
      }
      else
      {
         stringstream str;
         str << "Active Game list saved to Falcon (" << (int)activeGames.size() << " active games).  Original comHistory.dat file was backed up to comHistory.dat.orig.";

         AfxMessageBox(str.str().c_str());
      }
   }
   else
   {
      stringstream str;
      str << "Unable to retrieve list of Active Game Servers\n"
          << error;
      AfxMessageBox(str.str().c_str());
   }
}

void CFalconLobbyDlg::OnTrayAutoRetrieve()
{
   // toggle menu check state
   bool checked = (m_popupMenu->GetMenuState(ID_TRAY_AUTORETRIEVE,MF_BYCOMMAND) & MF_CHECKED) != 0;
   checked = !checked;

   if (checked)
   {
      if (AfxMessageBox("Enabling this option will automatically download the "
                        "list of Active Game Servers periodically (the amount "
                        "of time between downloads is configurable by editing "
                        "the settings file) and save them to Falcon.  The Falcon "
                        "comHistory.dat file will not be backed up when doing this.\n\n"
                        "Are you sure you want to do this?",
                        MB_YESNO|MB_ICONQUESTION) == IDNO)
      {
         return;
      }
   }

   m_popupMenu->CheckMenuItem(ID_TRAY_AUTORETRIEVE,MF_BYCOMMAND|
      (checked ? MF_CHECKED : MF_UNCHECKED));

   // store new value in settings file
   setSettingsInt("options","autoretrieve_active_servers",checked ? 1 : 0);
}

void CFalconLobbyDlg::OnTrayPublishMyGameServer()
{
   // does the settings file already contain game server info?
   if ((getSettingsString("gameserver","name").length() == 0) ||
       (getSettingsString("gameserver","token").length() == 0))
   {
      // no, prompt for server registration
      if (AfxMessageBox("It appears that you have not yet registered a game server.  "
                        "Registering your game server reserves the name you choose and allows "
                        "the up/down state to be reported to the Active Game Server list.  If you've "
                        "previously registered a game server and have re-installed this app, edit the "
                        "settings file and restore your backed up gameserver/name and gameserver/token "
                        "instead of registering a new game server.\n\n"
                        "Do you want to register a game server now?",MB_YESNO|MB_ICONQUESTION) == IDYES)
      {
         CRegisterGameServer dlg(this);
         dlg.DoModal();
      }
   }

   // does the settings file contain game server info?
   if ((getSettingsString("gameserver","name").length() != 0) &&
       (getSettingsString("gameserver","token").length() != 0))
   {
      // show server dialog
      if (m_serverDlg != NULL)
      {
         delete m_serverDlg;
      }
      m_serverDlg = new CServerDlg(this);
      m_serverDlg->ShowWindow(SW_SHOW);
      m_serverDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
   }
}

void CFalconLobbyDlg::OnTrayEditSettingsFile()
{
   // open the settings file using whatever app is associated with
   // the file-type (probably NotePad)
   ShellExecute(GetSafeHwnd(),"open",SETTINGS_FILE,0,0,SW_SHOW);
}

void CFalconLobbyDlg::OnTrayFalconHomePage()
{
   // navigate to the FalconLobby home page
   ShellExecute(GetSafeHwnd(),"open",APP_HOME_PAGE,0,0,SW_SHOW);
}

void CFalconLobbyDlg::OnTrayAbout()
{
   // show the about dialog
   CAboutDlg dlg;
   dlg.DoModal();
}

void CFalconLobbyDlg::OnTrayHelp()
{
   // build full path to help file using current directory
   const int bufsize = 4096;
   char buf[bufsize+1];
   memset(buf,0,bufsize+1);
   _getcwd(buf,bufsize);
   if (buf[strlen(buf)-1] != '\\')
   {
      strcat(buf,"\\");
   }
   strcat(buf,HELP_FILE);

   // build URL string
   stringstream str;
   str << "file://" << buf;

   ShellExecute(NULL,"open",str.str().c_str(),NULL,NULL,SW_SHOW);
}

void CFalconLobbyDlg::OnTrayExit()
{
   // close main window which will shutdown the application
   PostMessage(WM_CLOSE,0,0);
}
