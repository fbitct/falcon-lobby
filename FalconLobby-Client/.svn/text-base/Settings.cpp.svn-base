#include "stdafx.h"
#include "Settings.h"
#include "INIFile.h"
#include "const.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::map< std::pair<std::string,std::string>, int > Settings::minVals;

Settings::Settings()
{
   inifile = INIFile::getINIFile(SETTINGS_FILE);
}

Settings::~Settings()
{
}

std::string Settings::getSettingsString(std::string section, std::string name)
{
   return inifile->getString(section,name);
}

int Settings::getSettingsInt(std::string section, std::string name)
{
   int result = inifile->getInt(section,name);

   map<pair<string,string>,int>::iterator i;
   i = minVals.find(make_pair(section,name));
   if (i != minVals.end())
   {
      if (result < i->second)
      {
         result = i->second;
      }
   }

   return result;
}

void Settings::setSettingsString(std::string section, std::string name, std::string val)
{
   inifile->setString(section,name,val);
}

void Settings::setSettingsInt(std::string section, std::string name, int val)
{
   inifile->setInt(section,name,val);
}

void Settings::setSettingsMinInt(std::string section, std::string name, int minval)
{
   minVals[make_pair(section,name)] = minval;
}
