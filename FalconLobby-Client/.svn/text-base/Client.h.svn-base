#ifndef CLIENT_H
#define CLIENT_H

#include "HTTPClient.h"
#include "Settings.h"

#include <string>
#include <vector>

/**
 * handles interfacing with the game registry HTTP server on behalf of
 * the application by making use of the HTTPClient class to issue
 * requests and interpret responses from the server
 */
class Client: public Settings
{
public:

   /**
    * represents a game server
    */
   class Game
   {
   public:
      std::string game_name;
      std::string game_ip;
      std::string game_theater;
      unsigned long game_uptime;

      Game()
      {
         game_uptime = 0;
      }
   };

   Client();

   virtual ~Client();

   /** 
    * provides this class with the window that owns the tray icon; this allows
    * modifying the tray icon to indicate that a request is in progress
    */
   void setWnd(HWND hWnd);

   /**
    * registers a new game server, returning a game token to be used in subsequent calls
    * @param gameName name of the game server to register
    * @param gameToken game token returned upon successfull completion
    * @param error upon failure, this will provide a description of the error
    * @result true for success, false otherwise
    */
   bool registerNewGame(std::string gameName, std::string & gameToken, std::string & error);

   /**
    * set the state of the game on the server as specified using the token returned from a
    * previous call to registerNewGame
    * @param gameToken game token string returned from a successful call to registerNewGame
    * @param isUp true if game server is available, false if not
    * @param gameTheater theater the game is running in
    * @param error if this call fails, descriptive text for the error is provided here
    * @return true if success, false otherwise
    */
   bool setGameState(std::string gameToken, bool isUp, std::string gameTheater, std::string & error);

   /**
    * set the public info for a game.
    * @param gameToken game token string returned from a successful call to registerNewGame
    * @param gameHomePage home page URL
    * @param gameVoiceComm voice comms (teamspeak/ventrilo) URL
    * @param gameDesc game description
    * @param error if this call fails, descriptive text for the error is provided here
    * @return true if success, false otherwise
    */
   bool setGameInfo(std::string gameToken, std::string gameHomePage, std::string gameVoiceComm, std::string gameDesc, std::string & error);

   /**
    * retrieve active games
    * @param activeGames will provide the set of active games
    * @param error if this call fails, descriptive text for the error is provided here
    * @return true if success, false otherwise
    */
   bool getActiveGames(std::vector<Game> & activeGames, std::string & error);

private:
   HWND hWnd;

   /// class used to communicate with the server
   HTTPClient httpClient;

   /// used to submit requests for handling by the worker thread
   class Request
   {
   public:
      Request(Client & outer)
      : requestState(RS_PENDING),
        outer(outer)
      {
      }

      virtual ~Request()
      {
      }

      std::vector< std::pair<std::string,std::string> > formFields;

      typedef enum
      {
         RS_PENDING = 0,
         RS_FAILED,
         RS_SUCCEEDED
      } RequestState;

      RequestState requestState;

      std::string requestResponse;
      std::string requestError;

      Client & outer;
   };

   /**
    * submits a request for processing by a worker thread and blocks until the
    * request has completed or the configurable timeout has expired
    */
   void handleRequest(Request & request);

   /**
    * worker thread function for the handleRequest method
    */
   static DWORD WINAPI _handleRequest(LPVOID lpParameter);

   /**
    * parses response from the server, checking for success/failure and then providing the lines
    * of the remaining response data
    * @param response response received from the server
    * @param results will contain the result lines which follow the success/failure indication
    * @param error for error responses from the server, this contains the error message
    * @result true for success, false for failure in which case the error text will also be provided
    */
   bool parseServerResponse(std::string response, std::vector<std::string> & results, std::string & error);

   /**
    * builds the full URL used for submitting requests to the server
    * @return request URL
    */
   std::string getRequestURL();
};

#endif
