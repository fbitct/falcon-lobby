#include "stdafx.h"
#include "INIFile.h"
#include "Mutex.h"
#include "const.h"

#include <sstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::map<std::string,INIFile*> INIFile::files;

INIFile * INIFile::getINIFile(std::string filename)
{
   INIFile * result = NULL;

   Mutex m(INIFILE_MUTEX_NAME);

   // see if instance already exists for the specified filename
   std::map<std::string,INIFile*>::iterator i;
   i = files.find(filename);
   if (i != files.end())
   {
      // return existing instance
      result = i->second;
   }
   else
   {
      // return new instance and store for future requests
      result = new INIFile(filename);
      files[filename] = result;
   }

   return result;
}

void INIFile::cleanup()
{
   while (files.size() > 0)
   {
      delete files.begin()->second;
      files.erase(files.begin());
   }
}

INIFile::INIFile(string filename)
: filename(filename),
  filemodtime(0)
{
   // if a full pathname wasn't specified, build a full pathname
   // based on the current directory
   if (filename.find('\\') == string::npos)
   {
      const int bufsize = 4096;
      char buf[bufsize+1];
      memset(buf,0,bufsize+1);

      _getcwd(buf,bufsize);
      if (buf[strlen(buf)-1] != '\\')
      {
         strcat(buf,"\\");
      }

      strcat(buf,filename.c_str());
      this->filename = buf;
   }
}

INIFile::~INIFile()
{
}

string INIFile::getString(string section, string name)
{
   string result;

   Mutex m(INIFILE_MUTEX_NAME);

   // ensure latest INI file contents are loaded
   loadFile();

   // lookup section
   map<string,map<string,string> >::iterator i;
   i = data.find(section);
   if (i != data.end())
   {
      // lookup name
      map<string,string>::iterator j;
      j = i->second.find(name);
      if (j != i->second.end())
      {
         // return the value
         result = j->second;
      }
   }

   return result;
}

int INIFile::getInt(std::string section, std::string name)
{
   int result = 0;

   try
   {
      string strValue = getString(section,name);

      istringstream iss(strValue);
      int tmp;
      iss >> tmp;
      result = tmp;
   }

   catch (...)
   {
   }

   return result;
}

void INIFile::loadFile()
{
   // verify the file exists and that we can retrieve info on it
   struct _stat s;
   if (_stat(filename.c_str(),&s) == 0)
   {
      // check if file mod time more recent than last loaded mod time
      if (s.st_mtime > filemodtime)
      {
         filemodtime = s.st_mtime;

         const int secbufsize = 128 * 1024;
         char secbuf[secbufsize];
         memset(secbuf,0,secbufsize);

         // load section names
         GetPrivateProfileSectionNames(secbuf,secbufsize,filename.c_str());

         // for each section
         for (char * section = secbuf; *section != '\0'; section = &section[strlen(section)+1])
         {
            map<string,string> sectionValues;

            const int databufsize = secbufsize;
            char databuf[databufsize];
            memset(databuf,0,databufsize);

            // load names and values in this section
            GetPrivateProfileSection(section,databuf,databufsize,filename.c_str());

            // for each entry
            for (char * entry = databuf; *entry != '\0'; entry = &entry[strlen(entry)+1])
            {
               // copy entry to tmp buffer
               const int entrybufsize = 1024;
               char entrybuf[entrybufsize];
               memset(entrybuf,0,entrybufsize);
               strcpy(entrybuf,entry);

               // split entry into separate name/value strings
               char * sep = strstr(entrybuf,"=");
               if (sep != NULL)
               {
                  *sep++ = '\0';

                  // store this entry
                  sectionValues[entrybuf] = sep;
               }
            }

            // update data with the entries in this section
            data[section] = sectionValues;
         }
      }
   }
   else
   {
      // unable to retrieve status on the file, clear internal data
      data.clear();      
   }
}

void INIFile::setString(std::string section, std::string name, std::string val)
{
   Mutex m(INIFILE_MUTEX_NAME);

   WritePrivateProfileString(section.c_str(),name.c_str(),val.c_str(),filename.c_str());
}

void INIFile::setInt(std::string section, std::string name, int val)
{
   stringstream str;
   str << val;
   setString(section,name,str.str());
}

