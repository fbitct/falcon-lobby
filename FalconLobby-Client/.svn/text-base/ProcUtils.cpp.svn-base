#include "stdafx.h"
#include "const.h"
#include "ProcUtils.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ProcUtils::ProcUtils()
{
}

ProcUtils::~ProcUtils()
{
}

vector<string> ProcUtils::getRunningProcessModuleNames()
{
   vector<string> result;

   // create snapshot of all processes
   HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
   if (hProcessSnapshot != INVALID_HANDLE_VALUE)
   {
      PROCESSENTRY32 pe32;
      pe32.dwSize = sizeof(pe32);

      // loop through all processes in the snapshot      
      if (Process32First(hProcessSnapshot,&pe32))
      {
         do
         {
            // create a snapshot of all modules in this process
            HANDLE hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pe32.th32ProcessID);
            if (hModuleSnapshot != INVALID_HANDLE_VALUE)
            {
               MODULEENTRY32 me32;
               me32.dwSize = sizeof(me32);

               // get first module
               if (Module32First(hModuleSnapshot, &me32))
               {
                  // add to results
                  result.push_back(me32.szModule);
               }

               // release the snapshot data
               CloseHandle(hModuleSnapshot);
            }

         } while (Process32Next(hProcessSnapshot,&pe32));
      }

      // release the snapshot data
      CloseHandle(hProcessSnapshot);
   }

   return result;
}

bool ProcUtils::isProcessRunning(std::string processModuleName)
{
   bool result = false;

   vector<string> procs = getRunningProcessModuleNames();

   vector<string>::iterator i;
   for (i = procs.begin(); (!result) && (i != procs.end()); ++i)
   {
      if (processModuleName == *i)
      {
         result = true;
      }
   }

   return result;
}



