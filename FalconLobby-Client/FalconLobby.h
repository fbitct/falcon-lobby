// FalconLobby.h : main header file for the FalconLobby application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CFalconLobbyApp:
// See FalconLobby.cpp for the implementation of this class
//

class CFalconLobbyApp : public CWinApp
{
public:
	CFalconLobbyApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFalconLobbyApp theApp;