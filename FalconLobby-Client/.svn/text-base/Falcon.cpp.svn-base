#include "stdafx.h"
#include "Falcon.h"
#include "const.h"

#include <sstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Falcon::Falcon()
: processRunning(false),
  portsInUse(false),
  rulFileTouched(false),
  hDirMonitor(INVALID_HANDLE_VALUE)
{
   // start the worker threads

   AfxBeginThread(_threadFunc_processCheck,(LPVOID)this);
   AfxBeginThread(_threadFunc_portCheck,(LPVOID)this);
   AfxBeginThread(_threadFunc_rulFileCheck,(LPVOID)this);
}

Falcon::~Falcon()
{
}

bool Falcon::isMPServerRunning()
{
   return (processRunning && portsInUse && rulFileTouched);
}

UINT Falcon::_threadFunc_processCheck(LPVOID lpParm)
{
   return ((Falcon*)lpParm)->threadFunc_processCheck();
}

UINT Falcon::_threadFunc_portCheck(LPVOID lpParm)
{
   return ((Falcon*)lpParm)->threadFunc_portCheck();
}

UINT Falcon::_threadFunc_rulFileCheck(LPVOID lpParm)
{
   return ((Falcon*)lpParm)->threadFunc_rulFileCheck();
}

UINT Falcon::threadFunc_processCheck()
{
   DWORD processCheckLastTime = 0;

   while (true)
   {
      DWORD now = GetTickCount();

      if ((now - processCheckLastTime) > PROCESS_STATE_CHECK_PERIOD_MS)
      {
         processCheckLastTime = now;

         // is the Falcon process running?
         processRunning = m_procUtils.isProcessRunning(getSettingsString("Falcon","process_name"));

         // if the process is not running, clear the rulFile flag
         if (!processRunning)
         {
            rulFileTouched = false;
         }
      }

      Sleep(WORKER_THREAD_LOOP_PERIOD_MS);
   }

   AfxEndThread(0,TRUE);

   return 0;
}

UINT Falcon::threadFunc_portCheck()
{
   DWORD portCheckLastTime = 0;

   while (true)
   {
      DWORD now = GetTickCount();

      if ((now - portCheckLastTime) > PORT_STATE_CHECK_PERIOD_MS)
      {
         // are the Falcon ports in use?
         portsInUse = m_netUtils.portInUse(getSettingsInt("Falcon","server_port1"),SOCK_DGRAM) &&
                     m_netUtils.portInUse(getSettingsInt("Falcon","server_port2"),SOCK_DGRAM);

         // if ports not in use, clear the rulFile flag
         if (!portsInUse)
         {
            rulFileTouched = false;
         }
      }

      Sleep(WORKER_THREAD_LOOP_PERIOD_MS);
   }

   AfxEndThread(0,TRUE);

   return 0;
}

UINT Falcon::threadFunc_rulFileCheck()
{
   while (true)
   {
      // get config dir path
      string configDir;
      if (getConfigDir(configDir))
      {
         // start monitoring the config directory for changes
         hDirMonitor = FindFirstChangeNotification(
            configDir.c_str(),
            FALSE,
            FILE_NOTIFY_CHANGE_SIZE |
            FILE_NOTIFY_CHANGE_LAST_WRITE |
            FILE_NOTIFY_CHANGE_FILE_NAME |
            FILE_NOTIFY_CHANGE_ATTRIBUTES);

         if (hDirMonitor != INVALID_HANDLE_VALUE)
         {
            while (true)
            {
               // wait for a change event in the config directory
               DWORD waitStatus = WaitForSingleObject(hDirMonitor,INFINITE);
               if (waitStatus == WAIT_OBJECT_0)
               {
                  // the event indicates something changed in the directory, but
                  // it is now necessary to find out if a .rul file was changed

                  stringstream config_rul;
                  config_rul << configDir << "\\*.rul";

                  // search for all .rul files in the config directory
                  WIN32_FIND_DATA wfd;
                  HANDLE hSearch = FindFirstFile(config_rul.str().c_str(),&wfd);
                  if (hSearch != INVALID_HANDLE_VALUE)
                  {
                     do
                     {
                        stringstream rulFileStr;
                        rulFileStr << configDir << "\\" << wfd.cFileName;
                        string rulFile = rulFileStr.str();

                        // check last modification time for the file
                        struct _stat s;
                        if (_stat(rulFile.c_str(),&s) == 0)
                        {
                           bool fileTouched = false;

                           // check if this is an added file or a newly modified file

                           map<string,time_t>::iterator i;
                           i = modTimes.find(rulFile);

                           // added file
                           if (i == modTimes.end())
                           {
                              fileTouched = true;
                              modTimes[rulFile] = s.st_mtime;
                           }
                           else
                           {
                              // modified file
                              if (s.st_mtime > modTimes[rulFile])
                              {
                                 fileTouched = true;
                                 modTimes[rulFile] = s.st_mtime;
                              }
                           }

                           // touched file?
                           if (fileTouched)
                           {
                              // if the file was touched while the process was running
                              // and the ports were in use, then set the rulFileTouched flag
                              if (processRunning && portsInUse)
                              {
                                 rulFileTouched = true;
                              }
                           }
                        }

                     } while (FindNextFile(hSearch,&wfd) != 0);

                     FindClose(hSearch);
                  }

                  // begin waiting for the next change event
                  FindNextChangeNotification(hDirMonitor);
               }
               else
               {
                  Sleep(WORKER_THREAD_LOOP_PERIOD_MS);
               }
            }
         }
      }

      Sleep(RULFILEMONITOR_RETRYDELAY_MS);
   }

   AfxEndThread(0,TRUE);

   return 0;
}

bool Falcon::getConfigDir(std::string & configDir)
{
   bool result = false;
   configDir = "";

   // generate path to the config dir using registry entries

   HKEY hKey;
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,getSettingsString("Falcon","instdir_regkey").c_str(),0,KEY_READ,&hKey) == ERROR_SUCCESS)
   {
      const int maxpathlen = 4096;
      char pathbuf[maxpathlen+1];
      memset(pathbuf,0,maxpathlen+1);
      DWORD vType;
      DWORD bufSize = maxpathlen;

      if (RegQueryValueEx(hKey,getSettingsString("Falcon","instdir_regval").c_str(),0,&vType,(LPBYTE)pathbuf,&bufSize) == ERROR_SUCCESS)
      {
         stringstream ss;
         ss << pathbuf << getSettingsString("Falcon","config_path");

         configDir = ss.str();
         result = true;
      }

      RegCloseKey(hKey);
   }

   return result;
}

bool Falcon::getOptionsFile(std::string & optionsFile)
{
   bool result = false;
   optionsFile = "";

   // generate path to the options file using registry entries

   HKEY hKey;
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,getSettingsString("Falcon","instdir_regkey").c_str(),0,KEY_READ,&hKey) == ERROR_SUCCESS)
   {
      const int maxpathlen = 4096;
      char pathbuf[maxpathlen+1];
      memset(pathbuf,0,maxpathlen+1);
      DWORD vType;
      DWORD bufSize = maxpathlen;

      if (RegQueryValueEx(hKey,getSettingsString("Falcon","instdir_regval").c_str(),0,&vType,(LPBYTE)pathbuf,&bufSize) == ERROR_SUCCESS)
      {
         stringstream ss;
         ss << pathbuf << getSettingsString("Falcon","options_path");

         optionsFile = ss.str();
         result = true;
      }

      RegCloseKey(hKey);
   }

   return result;
}

string Falcon::getTheater()
{
   string result;

   // get path to options file
   string optionsFile;
   if (getOptionsFile(optionsFile))
   {
      // open options file
      FILE * f = fopen(optionsFile.c_str(),"r");
      if (f != NULL)
      {
         // read through entire file, searching for theater variable
         bool gotTheater = false;
         while ( (!gotTheater) && (!feof(f)) )
         {
            char line[_MAX_PATH+1];
            memset(line,0,_MAX_PATH+1);

            fgets(line,_MAX_PATH,f);

            char * eq = strstr(line,"=");
            if (eq != NULL)
            {
               char name[_MAX_PATH+1];
               char val[_MAX_PATH+1];
               memset(name,0,_MAX_PATH+1);
               memset(val,0,_MAX_PATH+1);

               strcpy(val,&eq[1]);
               *eq = '\0';
               strcpy(name,line);

               if (name[strlen(name)-1] == '\n')
               {
                  name[strlen(name)-1] = '\0';
               }

               if (val[strlen(val)-1] == '\n')
               {
                  val[strlen(val)-1] = '\0';
               }

               // does this line contain the theatre variable?
               if (strcmp(name,THEATERNAME_VAR) == 0)
               {
                  result = val;
                  gotTheater = true;
               }
            }
         }

         fclose(f);
      }
   }

   return result;
}