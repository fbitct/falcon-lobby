#ifndef PROCUTILS_H
#define PROCUTILS_H

#include <string>
#include <vector>

/**
 * Provides utility methods for working with Windows processes
 */
class ProcUtils
{
public:
   ProcUtils();
   virtual ~ProcUtils();

   /**
    * retrieves the names of all the running processes
    * @return vector of strings, each one being the module name of a running process
    */
   std::vector<std::string> getRunningProcessModuleNames();

   /**
    * determines if a process with the specified module name is running
    * @param processModuleName the process module name
    * @return bool true if the process is running, false otherwise
    */
   bool isProcessRunning(std::string processModuleName);
};

#endif
