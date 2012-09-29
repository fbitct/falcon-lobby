#include "stdafx.h"
#include "const.h"
#include "NetUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI testFunc(LPVOID parm)
{
   // initialize WinSock
   WSADATA wsaData;
   WSAStartup(MAKEWORD(WINSOCK_MAJOR_VERSION, WINSOCK_MINOR_VERSION),&wsaData);

   SOCKET watchSock = socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
   if (watchSock != INVALID_SOCKET)
   {
      TRACE0("SOCK: created watch socket\n");

      struct sockaddr_in saddr;
      memset(&saddr,0,sizeof(saddr));
      saddr.sin_family = AF_INET;
      saddr.sin_port = htons(2934);
      saddr.sin_addr.S_un.S_addr = INADDR_ANY;

      int berr = bind(watchSock,(struct sockaddr *)&saddr,sizeof(saddr));
      if (berr == SOCKET_ERROR)
      {
         TRACE1("SOCK: bind() failed, %d\n", WSAGetLastError());
      }
      else
      {
         while (true)
         {
            fd_set readfds;
            readfds.fd_count = 1;
            readfds.fd_array[0] = watchSock;

            TRACE0("SOCK: blocking for availability of data\n");
            int selerr = select(1,&readfds,0,0,NULL);
            if (selerr == 0)
            {
               TRACE0("SOCK: select() returned 0\n");
            }
            else if (selerr == SOCKET_ERROR)
            {
               TRACE1("SOCK: select() returned SOCKET_ERROR, %d\n", WSAGetLastError());
            }
            else
            {
               struct sockaddr_in addr;
               int addrsz = sizeof(addr);
               const int buflen = 30000;
               char buf[buflen];

               TRACE0("SOCK: data available, reading now\n");

               int err = recvfrom(watchSock,buf,buflen,0,(struct sockaddr *)&addr,&addrsz);
               if (err == 0)
               {
                  TRACE0("SOCK: closed\n");
                  break;
               }
               else if (err == SOCKET_ERROR)
               {
                  TRACE1("SOCK: SOCKET_ERROR, %d\n", WSAGetLastError());
               }
               else
               {
                  TRACE1("SOCK: received %d bytes\n", err);
               }
            }

            Sleep(500);
         }
      }
   }
   else
   {
      int err = WSAGetLastError();

      TRACE1("SOCK: failed to create watch socket, %d\n", err);
   }

   return 0;
}

DWORD WINAPI testFunc2(LPVOID parm)
{
   // initialize WinSock
   WSADATA wsaData;
   WSAStartup(MAKEWORD(WINSOCK_MAJOR_VERSION, WINSOCK_MINOR_VERSION),&wsaData);

   SOCKET sendSock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
   if (sendSock != INVALID_SOCKET)
   {
      TRACE0("SOCK2: created send socket\n");

      struct sockaddr_in saddr;
      memset(&saddr,0,sizeof(saddr));
      saddr.sin_family = AF_INET;
      saddr.sin_port = htons(2934);
      saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

      TRACE0("SOCK2: waiting to send\n");
      Sleep(60000);

      char buf[111];
      FILE * f = fopen("c:\\request_connection_details","rb");
      if (f != NULL)
      {
         if (fread(buf,111,1,f) == 1)
         {
            TRACE0("SOCK2: sending data\n");

            int sterr = sendto(sendSock,buf,111,0,(struct sockaddr *)&saddr,sizeof(saddr));
            if (sterr == 111)
            {
               TRACE0("SOCK2: sent packet\n");


            fd_set readfds;
            readfds.fd_count = 1;
            readfds.fd_array[0] = sendSock;

               TRACE0("SOCK2: waiting for response\n");
            int selerr = select(1,&readfds,0,0,NULL);
            if (selerr == 0)
            {
               TRACE0("SOCK2: select() returned 0\n");
            }
            else if (selerr == SOCKET_ERROR)
            {
               TRACE1("SOCK2: select() returned SOCKET_ERROR, %d\n", WSAGetLastError());
            }
            else
            {
               struct sockaddr_in addr;
               int addrsz = sizeof(addr);
               const int buflen = 30000;
               char buf[buflen];

               TRACE0("SOCK2: data available, reading now\n");

               int err = recvfrom(sendSock,buf,buflen,0,(struct sockaddr *)&addr,&addrsz);
               if (err == 0)
               {
                  TRACE0("SOCK2: closed\n");
               }
               else if (err == SOCKET_ERROR)
               {
                  TRACE1("SOCK2: SOCKET_ERROR, %d\n", WSAGetLastError());
               }
               else
               {
                  TRACE1("SOCK2: received %d bytes\n", err);
               }
            }


            }
            else
            {
               TRACE0("SOCK: error sending packet\n");
            }
         }
         else
         {
            TRACE0("SOCK: error reading packet file\n");
         }

         fclose(f);
      }
      else
      {
         TRACE0("SOCK: failed to open packet file\n");
      }

      closesocket(sendSock);
   }
   else
   {
      int err = WSAGetLastError();

      TRACE1("SOCK: failed to create send socket, %d\n", err);
   }

   return 0;
}

NetUtils::NetUtils()
: wsInitialized(false)
{
   // initialize WinSock
   WSADATA wsaData;
   wsInitialized = WSAStartup(MAKEWORD(WINSOCK_MAJOR_VERSION, WINSOCK_MINOR_VERSION),&wsaData) == 0;

   /*
   DWORD threadID = 0;
   CreateThread(0,0,testFunc,0,0,&threadID);
   TRACE1("SOCK: created listener thread 0x%x\n", threadID);

   DWORD threadID2 = 0;
   CreateThread(0,0,testFunc2,0,0,&threadID2);
   TRACE1("SOCK: created sender thread 0x%x\n", threadID2);
   */
}

NetUtils::~NetUtils()
{
   // cleanup WinSock only if it was successfully initialized
   if (wsInitialized)
   {
      WSACleanup();
   }
}

bool NetUtils::portInUse(int port, int protocol)
{
	bool result = false;

   // create socket
   SOCKET sock = socket(AF_INET,protocol,0);
	if (sock != INVALID_SOCKET)
	{
      // initialize address structure
	   struct sockaddr_in addr;
	   memset(&addr,0,sizeof(addr));
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.S_un.S_addr = INADDR_ANY;

      // attempt to bind to the specified port
		int berr = bind(sock,(sockaddr*)&addr,sizeof(addr));
		if (berr == SOCKET_ERROR)
		{
			DWORD lerr = GetLastError();
			if (lerr == WSAEADDRINUSE)
			{
            // bind failed, port was in use already
				result = true;
			}
		}

      // close the socket
		closesocket(sock);
	}

   return result;
}

