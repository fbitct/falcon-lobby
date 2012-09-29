#ifndef FALCON_H
#define FALCON_H

#include "NetUtils.h"
#include "ProcUtils.h"
#include "Settings.h"

#include <string>
#include <map>

/**
 * provides code which creates a logical interface with the Falcon executable
 */
class Falcon : public Settings
{
public:
	Falcon();
	virtual ~Falcon();
	
   /**
    * used to determine if a local multiplayer game server is currently running
    */
   bool isMPServerRunning();

   /**
    * provides the current theater of operation
    */
   std::string getTheater();

private:

   //
   // flags which are updated by the worker threads and used to determine
   // whether or not a MP game server is running
   //

   bool processRunning;
   bool portsInUse;
   bool rulFileTouched;

   // utility classes
   NetUtils m_netUtils;
   ProcUtils m_procUtils;

   //
   // threads which handle checking for server status
   //

   static UINT _threadFunc_processCheck(LPVOID lpParm);
   UINT threadFunc_processCheck();

   static UINT _threadFunc_portCheck(LPVOID lpParm);
   UINT threadFunc_portCheck();

   static UINT _threadFunc_rulFileCheck(LPVOID lpParm);
   UINT threadFunc_rulFileCheck();

   /**
    * provides the path to the Falcon config directory
    */
   bool getConfigDir(std::string & configDir);

   /**
    * provides the path to the options file
    */
   bool getOptionsFile(std::string & optionsFile);

   /// handle used for directory change monitoring
   HANDLE hDirMonitor;

   /// last mod times for .rul files in config directory
   std::map<std::string,time_t> modTimes;
};

#endif
