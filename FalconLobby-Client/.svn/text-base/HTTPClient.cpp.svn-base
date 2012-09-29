#include "stdafx.h"
#include "HTTPClient.h"
#include "const.h"

#include <sstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HTTPClient::HTTPClient()
: hInternet(NULL),
  auth_enabled(false)
{
   // create a top-level internet handle
	hInternet = InternetOpen(USER_AGENT,INTERNET_OPEN_TYPE_PRECONFIG,0,0,0);
}

HTTPClient::~HTTPClient()
{
   // release the top level internet handle
	if (hInternet != NULL)
	{
		InternetCloseHandle(hInternet);
		hInternet = NULL;
	}
}

void HTTPClient::enableAuthentication(bool enabled, string username, string password)
{
   auth_enabled = enabled;
   if (auth_enabled)
   {
      auth_username = username;
      auth_password = password;
   }
   else
   {
      auth_username = "";
      auth_password = "";
   }
}

bool HTTPClient::get(string url, string & response, string & error)
{
	bool result = false;
	response = "";
	error = "";

   // initialized?
	if (hInternet != NULL)
	{
      // set auth fields if applicable
      if (auth_enabled)
      {
         InternetSetOption(hInternet,INTERNET_OPTION_USERNAME,(LPVOID)((char*)(auth_username.c_str())),(DWORD)auth_username.size()+1);
         InternetSetOption(hInternet,INTERNET_OPTION_PASSWORD,(LPVOID)((char*)(auth_password.c_str())),(DWORD)auth_password.size()+1);
      }

		DWORD dwFlags = INTERNET_FLAG_NO_CACHE_WRITE |
                      INTERNET_FLAG_NO_UI |
                      INTERNET_FLAG_PRAGMA_NOCACHE |
                      INTERNET_FLAG_RELOAD;

      // open a handle to the URL
		HINTERNET hURL = InternetOpenUrl(hInternet,url.c_str(),0,0,dwFlags,0);
      if (hURL != NULL)
      {
         // retrieve status code
         DWORD statusCode = 0;
         DWORD statusCodeSize = sizeof(statusCode);
         if (HttpQueryInfo(hURL,HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER,
            &statusCode,&statusCodeSize,NULL))
         {
            // check request status code
            if (statusCode == HTTP_STATUS_OK)
            {
               // read the response
               result = getInternetResponse(hURL, response, error);
            }
            else
            {
               stringstream str;
               str << "server request failed ("
                   << statusCode
                   << ")";
               error = getWinInetErrorString(str.str());
            }
         }
         else
         {
            error = getWinInetErrorString("unable to retrieve request status code");
         }

         InternetCloseHandle(hURL);
      }
      else
      {
         error = getWinInetErrorString("server request failed");
      }
	}
	else
	{
		error = "not initialized";
	}

	return result;
}

bool HTTPClient::post(string url, vector< pair<string,string> > formFields, string & response, string & error)
{
	bool result = false;
	response = "";
	error = "";

   // initialized?
	if (hInternet != NULL)
	{
      // setup necessary variables to receive broken down URL
      const int maxslen = 1024;      
      char hostName[maxslen+1];
      char urlPath[maxslen+1];
      char extraInfo[maxslen+1];
      memset(hostName,0,maxslen+1);
      memset(urlPath,0,maxslen+1);
      memset(extraInfo,0,maxslen+1);
      URL_COMPONENTS urlComponents;
      memset(&urlComponents,0,sizeof(urlComponents));
      urlComponents.dwStructSize = sizeof(urlComponents);
      urlComponents.lpszHostName = hostName;
      urlComponents.dwHostNameLength = maxslen;
      urlComponents.lpszUrlPath = urlPath;
      urlComponents.dwUrlPathLength = maxslen;
      urlComponents.lpszExtraInfo = extraInfo;
      urlComponents.dwExtraInfoLength = maxslen;

      // break URL down into its pieces
      if (InternetCrackUrl(url.c_str(),0,0,&urlComponents))
      {
         // connect to the server
         HINTERNET hConn = InternetConnect(hInternet,hostName,urlComponents.nPort,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);
         if (hConn != NULL)
         {
            // set auth fields if applicable
            if (auth_enabled)
            {
               InternetSetOption(hConn,INTERNET_OPTION_USERNAME,(LPVOID)((char*)(auth_username.c_str())),(DWORD)auth_username.size()+1);
               InternetSetOption(hConn,INTERNET_OPTION_PASSWORD,(LPVOID)((char*)(auth_password.c_str())),(DWORD)auth_password.size()+1);
            }

		      DWORD dwFlags = INTERNET_FLAG_NO_CACHE_WRITE |
                            INTERNET_FLAG_NO_UI |
                            INTERNET_FLAG_PRAGMA_NOCACHE |
                            INTERNET_FLAG_RELOAD;

            // open the HTTP request
            HINTERNET hRequest = HttpOpenRequest(hConn,"POST",urlPath,NULL,NULL,NULL,dwFlags,NULL);
            if (hRequest != NULL)
            {
               // generate randomized boundary string for this request
               string boundary = generateMultipartBoundary();

               // request header notifies HTTP server we'll be providing multi-part form data
               // and notifies it of the boundary string we'll be using
               string contentTypeHeader = "Content-Type: multipart/form-data; boundary=" +
                  boundary;

               // add headers
               if (HttpAddRequestHeaders(hRequest,contentTypeHeader.c_str(),-1L,HTTP_ADDREQ_FLAG_ADD))
               {
                  // format form fields
                  stringstream str;
                  vector< pair<string,string> >::iterator i;
                  for (i = formFields.begin(); i != formFields.end(); ++i)
                  {
                     str << "--" << boundary << "\r\n"
                        << "Content-Disposition: form-data; name=\"" << i->first << "\"\r\n\r\n"
                        << i->second << "\r\n";
                  }
                  str << "--" << boundary << "--\r\n";

                  // send the request
                  string optionalData = str.str();
                  if (HttpSendRequest(hRequest,NULL,0,(LPVOID)optionalData.c_str(),(DWORD)optionalData.size()))
                  {
                     // retrieve status code
                     DWORD statusCode = 0;
                     DWORD statusCodeSize = sizeof(statusCode);
                     if (HttpQueryInfo(hRequest,HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER,
                        &statusCode,&statusCodeSize,NULL))
                     {
                        // check request status code
                        if (statusCode == HTTP_STATUS_OK)
                        {
                           // read the response
                           result = getInternetResponse(hRequest, response, error);
                        }
                        else
                        {
                           stringstream str;
                           str << "server request failed ("
                              << statusCode
                              << ")";
                           error = getWinInetErrorString(str.str());
                        }
                     }
                     else
                     {
                        error = getWinInetErrorString("unable to retrieve request status code");
                     }
                  }
                  else
                  {
                     error = getWinInetErrorString("unable to send HTTP request");
                  }
               }
               else
               {
                  error = getWinInetErrorString("unable to add request headers");
               }

               InternetCloseHandle(hRequest);
            }
            else
            {
               error = getWinInetErrorString("unable to open HTTP request");
            }

            InternetCloseHandle(hConn);
         }
         else
         {
            error = getWinInetErrorString("unable to connect to server");
         }
      }
      else
      {
         error = "invalid URL";
      }
	}
	else
	{
		error = "not initialized";
	}

	return result;
}

string HTTPClient::getWinInetErrorString(std::string msg)
{
   // Win32 API error code
   DWORD err = GetLastError();

   // Windows internet error response info
   const int maxerrtextlen = 4096;
   char errbuf[maxerrtextlen+1];
   memset(errbuf,0,maxerrtextlen+1);
   DWORD inerr = 0;
   DWORD errbufsize = maxerrtextlen;
   if (!InternetGetLastResponseInfo(&inerr,errbuf,&errbufsize))
   {
      sprintf(errbuf,"unknown error");
   }

   // put it all together, including the provided message
   stringstream str;
   if (msg.length() > 0) str << msg << " (";
   str << "err=" << err << ", inerr=" << inerr;
   if (strlen(errbuf) > 0)
   {
      str << ", \"" << errbuf << "\"";
   }
   if (msg.length() > 0) str << ")";

   return str.str();
}

bool HTTPClient::getInternetResponse(HINTERNET hFile, std::string & response, std::string & error)
{
   bool result = true;
   response = "";
   error = "";

   const int readbufsize = 4096;
   char readBuf[readbufsize+1];

   bool keepReading = true;
   DWORD bytesRead = 0;

   // continue reading data until an error occurs or reading has completed
   while (keepReading)
   {
      memset(readBuf,0,readbufsize+1);
      bytesRead = 0;

      // attempt to read more from the resource
      if (!InternetReadFile(hFile,readBuf,readbufsize,&bytesRead))
      {
         // error
         keepReading = false;
         result = false;
         response = "";
         error = getWinInetErrorString("problem reading response from the server");
      }
      else
      {
         // data returned, add to response and keep reading
         if (bytesRead > 0)
         {
            readBuf[bytesRead] = '\0';
            response += readBuf;
         }
         else
         {
            // no more data to read, finished!
            keepReading = false;
         }
      }
   }

   return result;
}

string HTTPClient::generateMultipartBoundary()
{
   string boundaryPrefix(27,'-');
   int r0 = rand() & 0xFFFF;
   int r1 = rand() & 0xFFFF;
   int r2 = rand() & 0xFFFF;

   char temp[13];
   sprintf(temp,"%04X%04X%04X", r0, r1, r2);

   return boundaryPrefix + temp;
}

