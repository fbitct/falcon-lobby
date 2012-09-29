/**
 * contains all the hard-coded constants used by the application
 */

#ifndef CONST_H
#define CONST_H

// product name and version used throughout the code
#define PRODUCT_NAME    "FalconLobby"
#define PRODUCT_VERSION "v1.1"
#define PRODUCT_DATE    "2006.08.04"

// caption of the main window, used to find the window
#define MAINWND_TITLE "FalconLobbyMainWnd"

// credits shown in About box
#define PRODUCT_CREDITS "Dave \"coder1024\" Calkins (coder1024@gmail.com)\n\nwww.falconlobby.com"

// home page
#define APP_HOME_PAGE "http://www.falconlobby.com"

// name of the settings file
#define SETTINGS_FILE "settings.ini"

// minimum download/upload periods (ms)
#define MIN_DLUL_PERIODS_MS 15000

// minimum request timeout (ms)
#define MIN_REQUESTTIMEOUT_MS 5000

// name of help file
#define HELP_FILE "help.htm"

// user agent used in HTTP requests
#define USER_AGENT "FalconLobby-Agent"

// combined with the HWND of the owning window, forms a unique ID for the icon
#define TRAY_ICON_ID  101

// message ID sent to window which owns the tray icon when events occur
#define TRAY_ICON_MSG (WM_APP+1)

// message ID sent to request a shutdown
#define APP_SHUTDOWN_MSG (WM_APP+2)

// shutdown loop delay
#define APP_SHUTDOWN_LOOP_DELAY_MS 500

// named mutexes
#define STARTUP_MUTEX_NAME       PRODUCT_NAME "::IsRunning"
#define INIFILE_MUTEX_NAME       PRODUCT_NAME "::INIFile::MUTEX"
#define CLIENTREQUEST_MUTEX_NAME PRODUCT_NAME "::Client::MUTEX"

// loop delay for request submission
#define REQUEST_SUBMIT_LOOP_DELAY_MS 200

// timer ID for periodic game list download
#define AUTO_DOWNLOAD_TIMER_ID 101

// loop delay for periodic game list download
#define AUTO_DOWNLOAD_LOOP_DELAY_MS 100

// timer ID for periodic game state upload
#define AUTO_UPLOAD_TIMER_ID 102

// loop delay for periodic game state upload
#define AUTO_UPLOAD_LOOP_DELAY_MS 100

// timer ID for periodic UI updates
#define UIUPDATE_TIMER_ID 103

// loop delay for periodic UI updates
#define UIUPDATE_LOOP_DELAY_MS 100

// timer ID for About Box icon flashing
#define ABOUTBOX_ICON_FLASH_TIMER_ID 101

// delay for about box icon flashing
#define ABOUTBOX_ICON_FLASHING_DELAY_MS 500

// timer ID for Server Dialog periodic display update
#define SERVERDLG_UPDATE_TIMER_ID 101

// delay for Server Dialog update timer
#define SERVERDLG_UPDATE_DELAY_MS 1000

// min request timeout
#define REQUEST_TIMEOUT_MIN_MS 1000

// max length of game server name
#define MAXLEN_GAMENAME 39

// max length of game server description
#define MAXLEN_GAMEDESC 255

// max length of game home page URL
#define MAXLEN_GAMEHOMEPAGE 128

// max length of game voice comms URL
#define MAXLEN_GAMEVOICECOMMS 128

// retry delay for rulFileMonitor thread if unable to determine config dir
#define RULFILEMONITOR_RETRYDELAY_MS 5000

// period for checking process state
#define PROCESS_STATE_CHECK_PERIOD_MS 5000

// period for checking port state
#define PORT_STATE_CHECK_PERIOD_MS 5000

// worker thread loop period
#define WORKER_THREAD_LOOP_PERIOD_MS 100

// backup warning text color in server info dialog
#define BACKUPWARNING_COLOR RGB(200,50,50)

// WinSock major and minor versions requested during initialization
#define WINSOCK_MAJOR_VERSION 2
#define WINSOCK_MINOR_VERSION 2

// stores list of IPs added to the phone book by this app
#define IMPORTEDIPSFILE "importedIPs"

// Falcon options.cfg file variable used to store current theater name
#define THEATERNAME_VAR "gs_curTheater"

#endif
