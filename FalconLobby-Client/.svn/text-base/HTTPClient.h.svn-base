#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <string>
#include <vector>
#include <utility>

/**
 * provides for interaction with an HTTP server using the Windows internet APIs
 */
class HTTPClient
{
public:
	HTTPClient();
	virtual ~HTTPClient();

   /**
    * enables/disables HTTP authentication for future requests using this instance
    * @param enabled true to enable authentication, false to disable authentication
    * @param username the username to use if enabled
    * @param password the password to use if enabled
    */
   void enableAuthentication(bool enabled, std::string username, std::string password);

	/**
	 * retrieves the specified URL using HTTP GET
	 * @param url url to retrieve
	 * @param response data received from the server is stored here
	 * @param error if this fails, a description of the error is provided here
	 * @return true if successfull, false if failed in which case error will contain more info
	 */
	bool get(std::string url, std::string & response, std::string & error);

	/**
	 * retrieves the specified URL using HTTP POST
	 * @param url url to retrieve
    * @param formFields form fields to submit as a vector of string name/value pairs
	 * @param response data received from the server is stored here
	 * @param error if this fails, a description of the error is provided here
	 * @return true if successfull, false if failed in which case error will contain more info
	 */
   bool post(std::string url, std::vector< std::pair<std::string,std::string> > formFields, std::string & response, std::string & error);

private:
   /// top-level handle used in MS internet API calls
	HINTERNET hInternet;

   /// tracks whether HTTP authentication is enabled
   bool auth_enabled;

   /// username used for authentication
   std::string auth_username;

   /// password used for authentication
   std::string auth_password;

   /**
    * enhanced get last error method which returns a formatted string
    * that includes GetLastError(), the result of InternetGetLastResponse()
    * and a specified message
    * @param msg custom message to be included in the result
    * @return formatted error string
    */
   std::string getWinInetErrorString(std::string msg);

   /**
    * reads the response from an internet handle and returns a string
    * containing the response
    * @param hFile internet handle (as returned from HttpOpenRequest or InternetOpenUrl)
    * @param response will contain the response on successful completion
    * @param error will contain an error description on failure
    * @return true if successful, false otherwise
    */
   bool getInternetResponse(HINTERNET hFile, std::string & response, std::string & error);

   /**
    * generates a randomized string to be used as a multipart boundary in a form HTTP POST
    * @return string multipart boundary
    */
   std::string generateMultipartBoundary();
};

#endif
