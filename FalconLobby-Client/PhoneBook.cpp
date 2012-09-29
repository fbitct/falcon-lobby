#include "stdafx.h"
#include "const.h"
#include "PhoneBook.h"

#include <string>
#include <sstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef unsigned char uchar;
typedef unsigned long ulong;

#define PROTOCOL      0
#define LAN_RATE      1
#define INTERNET_RATE 0
#define JETNET_RATE   0
#define IP_ADDRESS    0
#define FLAG          1
#define COUNT         1

// the file contains a 32-bit
// value which indicates the # of entries, followed by a record for each entry.
// the fields used for F4AF which need to be filled in are url (holds a string
// form of the server IP address), servername (friendly name), and password
// (server password)

// the key field is the url (IP address); if there are multiple entries in the
// phone book with the same url, only one of them will be displayed in Falcon

class PhoneBookRecord
{
public:
    unsigned long protocol;
	uchar		lan_rate;
	uchar		internet_rate;
	uchar		jetnet_rate;
	ulong		ip_address;				// Use this or the phone_number below
	char		url[40];		// Save the URL here, was Phone number, in string form.
	char		servername[40];
	char		theatername[20];
	char		password[33];
	char		flag;
	char		count;	

   PhoneBookRecord()
   {
      memset(this,0,sizeof(PhoneBookRecord));
      protocol = PROTOCOL;
      lan_rate = LAN_RATE;
      internet_rate = INTERNET_RATE;
      jetnet_rate = JETNET_RATE;
      ip_address = IP_ADDRESS;
      flag = FLAG;
      count = COUNT;
   }

   string toString()
   {
      stringstream ss;
      ss << "PhoneBookRecord { "
         << "protocol=" << protocol
         << ", lan_rate=" << (ulong)lan_rate
         << ", internet_rate=" << (ulong)internet_rate
         << ", jetnet_rate=" << (ulong)jetnet_rate;
      struct in_addr ia;
      memcpy(&ia,&ip_address,sizeof(ip_address));
      ss << ", ip_address=" << inet_ntoa(ia)
         << ", url=\"" << url << "\""
         << ", servername=\"" << servername << "\""
         << ", theatername=\"" << theatername << "\""
         << ", password=\"" << password << "\""
         << ", flag=" << (ulong)flag
         << ", count=" << (ulong)count
         << " }";
      return ss.str();
   }
};

PhoneBook::PhoneBook()
{
#ifdef _DEBUG
   /*
   string phoneBookFile;
   if (getPhoneBookFile(phoneBookFile))
   {
      // open the file for reading
      FILE * f = fopen(phoneBookFile.c_str(),"rb");
      if (f != NULL)
      {
         // get # entries
         ulong numEntries = 0;
         if (fread(&numEntries,sizeof(numEntries),1,f) == 1)
         {
            TRACE1("numEntries == %d\n", numEntries);

            // read all entries
            for (ulong x = 0; x < numEntries; x++)
            {
               PhoneBookRecord pbr;
               if (fread(&pbr,sizeof(pbr),1,f) != 1)
               {
                  TRACE0("ERROR: incomplete read\n");
                  break;
               }

               TRACE1("%s\n", pbr.toString().c_str());
            }

            // any extra entries?
            while (!feof(f))
            {
               PhoneBookRecord pbr;
               if (fread(&pbr,sizeof(pbr),1,f) == 1)
               {
                  TRACE1("EXTRA: %s\n", pbr.toString().c_str());
               }
            }
         }

         // close the file
         fclose(f);
      }
   }

   TRACE1("sizeof(PhoneBookRecord) == %d\n", sizeof(PhoneBookRecord));
   */
#endif
}

PhoneBook::~PhoneBook()
{
}

void PhoneBook::addEntry(Entry entry)
{
   entries.push_back(entry);
}

bool PhoneBook::commitChanges(bool createBackup, bool merge, std::string & err)
{
   bool result = false;
   err = "";

   // backup the file first if requested
   if (createBackup)
   {
      makeBackupCopy();
   }

   // get path to the file
   string phoneBookFile;
   if (getPhoneBookFile(phoneBookFile))
   {      
      // path to file which lists all IPs imported by this app
      char importedIPsFile[_MAX_PATH+1];
      memset(importedIPsFile,0,_MAX_PATH+1);
      GetModuleFileName(NULL,importedIPsFile,_MAX_PATH);
      char * bs = strrchr(importedIPsFile,'\\');
      bs[1] = '\0';
      strcat(importedIPsFile,IMPORTEDIPSFILE);

      // if merge is enabled, load the list of IPs previously imported
      // by this tool
      vector<string> importedIPs;
      if (merge)
      {
         FILE * f = fopen(importedIPsFile,"r");
         if (f != NULL)
         {
            while (!feof(f))
            {
               char lineBuf[_MAX_PATH+1];
               memset(lineBuf,0,_MAX_PATH+1);
               fscanf(f,"%s",lineBuf);
               if (strlen(lineBuf) > 0)
               {
                  importedIPs.push_back(lineBuf);
               }
            }

            fclose(f);
         }
      }

      // if merge is enabled, load existing phonebook and put aside any entries
      // we need to preserve (entries which were not imported with this app)
      vector<PhoneBookRecord> preservedEntries;
      if (merge)
      {
         // open the file for reading
         FILE * f = fopen(phoneBookFile.c_str(),"rb");
         if (f != NULL)
         {
            // get # entries
            ulong numEntries = 0;
            size_t sz = fread(&numEntries,sizeof(numEntries),1,f);
            if (sz == 1)
            {
               // read all entries
               for (ulong x = 0; x < numEntries; x++)
               {
                  PhoneBookRecord pbr;
                  if (fread(&pbr,sizeof(pbr),1,f) == 1)
                  {
                     // see if this IP is in the list of IPs imported by this app
                     bool isImportedIP = false;
                     vector<string>::iterator k;
                     for (k = importedIPs.begin(); (!isImportedIP) && (k != importedIPs.end()); ++k)
                     {
                        if (strcmp(pbr.url,k->c_str()) == 0)
                        {
                           isImportedIP = true;
                        }
                     }

                     // if entry was manually added by the user (not in the imported IP list)
                     if (!isImportedIP)
                     {
                        // ensure the IP address for this entry isn't in the
                        // list we're adding; if so, the list we're adding takes
                        // precedence
                        bool foundIt = false;
                        vector<Entry>::iterator i;
                        for (i = entries.begin(); (!foundIt) && (i != entries.end()); ++i)
                        {
                           if (strcmp(pbr.url,i->game_ip.c_str()) == 0)
                           {
                              foundIt = true;
                           }
                        }

                        // also make sure the IP address isn't already used by a manually
                        // added entries; a side effect of this is that it'll strip out
                        // manually added entries with duplicate IP addresses from the file;
                        // Falcon only displays one entry per IP anyway.
                        vector<PhoneBookRecord>::iterator j;
                        for (j = preservedEntries.begin(); (!foundIt) && (j != preservedEntries.end()); ++j)
                        {
                           if (strcmp(pbr.url,j->url) == 0)
                           {
                              foundIt = true;
                           }
                        }

                        if (!foundIt)
                        {
                           // put in the list of entries to preserve
                           preservedEntries.push_back(pbr);
                        }
                     }
                  }
                  else
                  {
                     break;
                  }
               }
            }

            // close the file
            fclose(f);
         }
      }

      // open the file for writing
      FILE * f = fopen(phoneBookFile.c_str(),"wb");
      if (f != NULL)
      {
         // write # entries
         ulong numEntries = (ulong)(entries.size()) + (ulong)(preservedEntries.size());
         fwrite(&numEntries,sizeof(numEntries),1,f);

         // write out the preserved entries first (if any)
         vector<PhoneBookRecord>::iterator j;
         for (j = preservedEntries.begin(); j != preservedEntries.end(); ++j)
         {
            fwrite(&(*j),sizeof(PhoneBookRecord),1,f);
         }

         // write out each entry
         vector<Entry>::iterator i;
         for (i = entries.begin(); i != entries.end(); ++i)
         {
            PhoneBookRecord pbr;
            strcpy(pbr.servername,i->game_name.c_str());
            strcpy(pbr.url,i->game_ip.c_str());
            strcpy(pbr.theatername,i->game_theater.c_str());

            fwrite(&pbr,sizeof(PhoneBookRecord),1,f);
         }

         // flush and close the file
         fflush(f);
         fclose(f);

         // write out list of imported IPs
         FILE * imps = fopen(importedIPsFile,"w");
         if (imps != NULL)
         {
            for (i = entries.begin(); i != entries.end(); ++i)
            {
               fprintf(imps,"%s\n",i->game_ip.c_str());
            }

            // flush and close
            fflush(imps);
            fclose(imps);
         }

         result = true;
      }
      else
      {
         err = "Unable to open the phone book file for writing; could be a file permissions issue or the file could be in use by the Falcon process";
      }
   }
   else
   {
      err = "Unable to determine location of phone book file; Falcon might not be installed correctly";
   }

   return result;
}

void PhoneBook::doInitialBackup()
{
   // build path for backup file
   char moduleFileName[_MAX_PATH+1];
   memset(moduleFileName,0,_MAX_PATH+1);
   GetModuleFileName(NULL,moduleFileName,_MAX_PATH);
   char * bs = strrchr(moduleFileName,'\\');
   bs[1] = '\0';
   CTime now = CTime::GetCurrentTime();
   CString nowstr = now.Format("comHistory.dat.%Y%m%d_%H%M%S");
   strcat(moduleFileName,nowstr);

   // path to original file
   string phoneBookFile;
   if (getPhoneBookFile(phoneBookFile))
   {
      // make backup copy
      CopyFile(phoneBookFile.c_str(),moduleFileName,FALSE);
   }
}

void PhoneBook::makeBackupCopy()
{
   // get path to the file
   string phoneBookFile;
   if (getPhoneBookFile(phoneBookFile))
   {
      // append ".orig" to path
      string pbFileOrig = phoneBookFile;
      pbFileOrig = pbFileOrig + ".orig";

      // make backup copy
      CopyFile(phoneBookFile.c_str(),pbFileOrig.c_str(),FALSE);
   }
}

bool PhoneBook::getPhoneBookFile(std::string & phoneBookFile)
{
   bool result = false;
   phoneBookFile = "";

   // generate path to the phone book file using registry entries

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
         ss << pathbuf << getSettingsString("Falcon","phonebook_path");
         
         phoneBookFile = ss.str();
         result = true;
      }

      RegCloseKey(hKey);
   }

   return result;
}

