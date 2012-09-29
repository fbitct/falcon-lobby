#ifndef NETUTILS_H
#define NETUTILS_H

/**
 * utility methods related to networking
 */
class NetUtils
{
public:

   NetUtils();
   virtual ~NetUtils();

   /**
    * used to determine if the specified port/protocol is already in use
    * @port port number
    * @protocol protocol number (you can use any value which is accepted as
    *           the protocol argument for the socket() function, for example
    *           SOCK_STREAM, SOCK_DGRAM)
    * @return true if the specified port/protocol is in use, false otherwise
    */
   bool portInUse(int port, int protocol);

private:
   /// indicates whether WinSock successfully initialized
   bool wsInitialized;
};

#endif
