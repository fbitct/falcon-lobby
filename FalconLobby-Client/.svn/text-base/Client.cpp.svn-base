#include "stdafx.h"
#include "resource.h"
#include "Client.h"
#include "Mutex.h"
#include "const.h"

#include <sstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Client::Client()
: hWnd(0)
{
}

Client::~Client()
{
}

void Client::setWnd(HWND hWnd)
{
   this->hWnd = hWnd;
}

bool Client::registerNewGame(string gameName, string & gameToken, string & error)
{
   bool result = false;
   gameToken = "";
   error = "";

   vector< pair<string,string> > formFields;
   formFields.push_back(make_pair("method","newgame"));
   formFields.push_back(make_pair("game_name",gameName));

   Request r(*this);
   r.formFields = formFields;

   handleRequest(r);

   if (r.requestState == Request::RS_SUCCEEDED)
   {
      vector<string> results;
      if (parseServerResponse(r.requestResponse,results,error))
      {
         gameToken = results.front();
         result = true;
      }
   }
   else
   {
      error = r.requestError;
   }

   return result;
}

bool Client::setGameState(string gameToken, bool isUp, string gameTheater, string & error)
{
   bool result = false;
   error = "";

   vector< pair<string,string> > formFields;
   formFields.push_back(make_pair("method","setgamestate"));
   formFields.push_back(make_pair("game_token",gameToken));
   formFields.push_back(make_pair("game_state",(isUp ? "up" : "down")));
   formFields.push_back(make_pair("game_theater",gameTheater));

   Request r(*this);
   r.formFields = formFields;

   handleRequest(r);

   if (r.requestState == Request::RS_SUCCEEDED)
   {
      vector<string> results;
      if (parseServerResponse(r.requestResponse,results,error))
      {
         result = true;
      }
   }
   else
   {
      error = r.requestError;
   }

   return result;
}

bool Client::setGameInfo(std::string gameToken, std::string gameHomePage, std::string gameVoiceComm, std::string gameDesc, std::string & error)
{
   bool result = false;
   error = "";

   vector< pair<string,string> > formFields;
   formFields.push_back(make_pair("method","setgameinfo"));
   formFields.push_back(make_pair("game_token",gameToken));
   formFields.push_back(make_pair("game_homepage",gameHomePage));
   formFields.push_back(make_pair("game_voicecomm",gameVoiceComm));
   formFields.push_back(make_pair("game_desc",gameDesc));

   Request r(*this);
   r.formFields = formFields;

   handleRequest(r);

   if (r.requestState == Request::RS_SUCCEEDED)
   {
      vector<string> results;
      if (parseServerResponse(r.requestResponse,results,error))
      {
         result = true;
      }
   }
   else
   {
      error = r.requestError;
   }

   return result;
}

bool Client::getActiveGames(vector<Game> & activeGames, std::string & error)
{
   bool result = false;
   error = "";
   activeGames.clear();

   vector< pair<string,string> > formFields;
   formFields.push_back(make_pair("method","getactivegames"));

   Request r(*this);
   r.formFields = formFields;

   handleRequest(r);

   if (r.requestState == Request::RS_SUCCEEDED)
   {
      vector<string> results;
      if (parseServerResponse(r.requestResponse,results,error))
      {
         result = true;

         //
         // results come back as a series of lines where a tag is used
         // to start each new game entry, and then name value alternating
         // lines are used to communicate fields
         //
         // Example:
         // game
         // game_name
         // my game
         // game_ip
         // 1.2.3.4
         // game_theater
         // Korea 2005
         // game_uptime
         // 120
         // game
         // game_name
         // bob's game
         // game_ip
         // 10.0.0.2
         // game_theater
         // Balkans
         // game_uptime
         // 34
         //

         Game g;
         bool pendingGame = false;
         vector<string>::iterator i;
         for (i = results.begin(); i != results.end(); ++i)
         {
            if (*i == "game")
            {
               if (pendingGame)
               {
                  activeGames.push_back(g);
                  Game _g;
                  g = _g;
                  pendingGame = false;
               }
            }
            else
            {
               if (*i == "game_name")
               {
                  g.game_name = *(++i);
                  pendingGame = true;
               }
               else if (*i == "game_ip")
               {
                  g.game_ip = *(++i);
                  pendingGame = true;
               }
               else if (*i == "game_theater")
               {
                  g.game_theater = *(++i);
                  pendingGame = true;
               }
               else if (*i == "game_uptime")
               {
                  g.game_uptime = atol((++i)->c_str());
                  pendingGame = true;
               }
            }
         }

         if (pendingGame)
         {
            activeGames.push_back(g);
         }
      }
   }
   else
   {
      error = r.requestError;
   }

   return result;
}

void Client::handleRequest(Request & request)
{
   request.requestState = Request::RS_PENDING;

   // set/clear authentication
   if (getSettingsInt("gameserver_registry","auth_enabled") != 0)
   {
      httpClient.enableAuthentication(true,
         getSettingsString("gameserver_registry","auth_username"),
         getSettingsString("gameserver_registry","auth_password"));
   }
   else
   {
      httpClient.enableAuthentication(false,"","");
   }

   // start the worker thread
   DWORD threadID = 0;
   HANDLE hThread = CreateThread(NULL,0,_handleRequest,(LPVOID)&request,0,&threadID);
   if (hThread == NULL)
   {
      request.requestState = Request::RS_FAILED;
      request.requestResponse = "";
      request.requestError = "unable to start worker thread";
   }
   else
   {
      DWORD startTime = GetTickCount();

      // get configurable timeout value
      int timeout = getSettingsInt("options","request_timeout_ms");
      if (timeout < REQUEST_TIMEOUT_MIN_MS)
      {
         timeout = REQUEST_TIMEOUT_MIN_MS;
      }

      bool flashState = true;

      // loop, waiting for either request completion (success or failure)
      // or until timeout has expired
      bool keepWaiting = true;
      while (keepWaiting)
      {
         Sleep(REQUEST_SUBMIT_LOOP_DELAY_MS);

         // flash the tray icon to indicate a request is in progress
         if (hWnd != 0)
         {
            NOTIFYICONDATA nid;
            memset(&nid,0,sizeof(nid));
            nid.cbSize           = sizeof(nid);
            nid.hWnd             = hWnd;
            nid.uID              = TRAY_ICON_ID;
            nid.hIcon            = AfxGetApp()->LoadIcon(flashState ? IDI_TRAY_ICON_LIT : IDI_TRAY_ICON);
            nid.uFlags           = NIF_ICON;
            Shell_NotifyIcon(NIM_MODIFY,&nid);
            
            flashState = !flashState;
         }

         {
            Mutex m(CLIENTREQUEST_MUTEX_NAME);

            // request is still pending and timeout has expired
            if ((request.requestState == Request::RS_PENDING) &&
                ((int)(GetTickCount() - startTime) > timeout))
            {
               // kill the thread
               TerminateThread(hThread,0);

               // set request results
               request.requestState = Request::RS_FAILED;
               request.requestResponse = "";
               request.requestError = "request timed out; no response from the server";

               // finished waiting
               keepWaiting = false;
            }
            else if ((request.requestState == Request::RS_SUCCEEDED) ||
                     (request.requestState == Request::RS_FAILED))
            {
               // request has completed; finished waiting
               keepWaiting = false;
            }
         }
      }

      CloseHandle(hThread);
   }

   // ensure icon is restored to original
   if (hWnd != 0)
   {
      NOTIFYICONDATA nid;
      memset(&nid,0,sizeof(nid));
      nid.cbSize           = sizeof(nid);
      nid.hWnd             = hWnd;
      nid.uID              = TRAY_ICON_ID;
      nid.hIcon            = AfxGetApp()->LoadIcon(IDI_TRAY_ICON);
      nid.uFlags           = NIF_ICON;
      Shell_NotifyIcon(NIM_MODIFY,&nid);      
   }
}

DWORD WINAPI Client::_handleRequest(LPVOID lpParameter)
{
   Request * request = (Request*)lpParameter;

   // submit the HTTP request
   string error;
   string response;
   if (request->outer.httpClient.post(request->outer.getRequestURL(),request->formFields,response,error))
   {
      // succeeded
      Mutex m(CLIENTREQUEST_MUTEX_NAME);
      request->requestState = Request::RS_SUCCEEDED;
      request->requestError = "";
      request->requestResponse = response;
   }
   else
   {
      // failed
      Mutex m(CLIENTREQUEST_MUTEX_NAME);
      request->requestState = Request::RS_FAILED;
      request->requestError = error;
      request->requestResponse = "";
   }

   return 0;
}

bool Client::parseServerResponse(string response, vector<string> & results, string & error)
{
   bool result = false;
   results.clear();
   error = "";
   string token;

   // split by line breaks
   bool gotFirstNonBlank = false;
   istringstream ss(response);
   while (!ss.eof())
   {
      getline(ss,token,'\n');
      
      if (!gotFirstNonBlank)
      {
         if (token.size() > 1)
         {
            gotFirstNonBlank = true;
         }
      }

      if (gotFirstNonBlank)
      {
         results.push_back(token);
      }
   }

   // split off first line which indicates status
   string status = results.front();
   results.erase(results.begin());

   // check status line
   istringstream sss(status);
   getline(sss,token,':');
   if (token == "SUCCESS")
   {
      result = true;
   }
   else
   {
      getline(sss,error,':');
   }
  
   return result;
}

std::string Client::getRequestURL()
{
   stringstream str;

   str << "http://"
       << getSettingsString("gameserver_registry","host")
       << ":" << getSettingsInt("gameserver_registry","port")
       << getSettingsString("gameserver_registry","request_path");

   return str.str();
}
