#ifndef SETTINGS_H
#define SETTINGS_H

class INIFile;

#include <string>
#include <map>

/**
 * handles providing info from the settings file; any class which requires settings file
 * info will derive from this class and then be able to access the get/set methods
 */
class Settings
{
public:
   Settings();
   virtual ~Settings();

   std::string getSettingsString(std::string section, std::string name);
   int getSettingsInt(std::string section, std::string name);

   void setSettingsString(std::string section, std::string name, std::string val);
   void setSettingsInt(std::string section, std::string name, int val);

   static void setSettingsMinInt(std::string section, std::string name, int minval);

private:
   INIFile * inifile;

   static std::map< std::pair<std::string,std::string>, int > minVals;
};

#endif
