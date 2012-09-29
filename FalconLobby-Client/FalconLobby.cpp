// FalconLobby.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FalconLobby.h"
#include "FalconLobbyDlg.h"
#include "const.h"
#include "PhoneBook.h"
#include "INIFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFalconLobbyApp

BEGIN_MESSAGE_MAP(CFalconLobbyApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFalconLobbyApp construction

CFalconLobbyApp::CFalconLobbyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFalconLobbyApp object

CFalconLobbyApp theApp;


// CFalconLobbyApp initialization

BOOL CFalconLobbyApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T(PRODUCT_NAME));

   // check for shutdown cmd line argument
   if (strstr(m_lpCmdLine,"-shutdown") != NULL)
   {
      // if the main app window exists, send it the shutdown message
      HWND hWnd;
      hWnd = ::FindWindow(NULL,MAINWND_TITLE);
      if (hWnd != NULL)
      {
         ::PostMessage(hWnd,APP_SHUTDOWN_MSG,0,0);
      }

      return FALSE;
   }

   // check for backup cmd line argument
   if (strstr(m_lpCmdLine,"-backup") != NULL)
   {
      PhoneBook pb;
      pb.doInitialBackup();

      return FALSE;
   }

   // ensure app is not already running
   CreateMutex(NULL,FALSE,STARTUP_MUTEX_NAME);
   if (GetLastError() == ERROR_ALREADY_EXISTS)
   {
      return FALSE;
   }

   // create main window
	CFalconLobbyDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

   // allow the INIFile class to cleanup it's instances
   INIFile::cleanup();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
