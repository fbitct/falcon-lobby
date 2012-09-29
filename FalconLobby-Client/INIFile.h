#ifndef INIFILE_H
#define INIFILE_H

#include <string>
#include <map>

class INIFile
{
public:

   /**
    * retrieve the INIFile instance for the specified filename
    * @param filename INI filename
    * @return INIFile instance for with the specified filename
    */
   static INIFile * getINIFile(std::string filename);

   /**
    * called during application shutdown to allow releasing
    * all used INI file objects
    */
   static void cleanup();

   // the below methods retrieve values in the desired type

   std::string getString(std::string section, std::string name);
   int getInt(std::string section, std::string name);

   // the below methods set values using the desired type

   void setString(std::string section, std::string name, std::string val);
   void setInt(std::string section, std::string name, int val);

private:
   /// INI filename
   std::string filename;

   /// file mod time when file was last loaded
   time_t filemodtime;

   /// section name => name => value
   std::map<std::string,std::map<std::string,std::string> > data;

   INIFile(std::string filename);
   virtual ~INIFile();

   // filename => INIFile instance
   static std::map<std::string,INIFile*> files;

   /**
    * ensures the latest data in the file is loaded into the data
    * variable.  if the file has never been loaded, or it has been
    * modified since it was last loaded, it will be loaded
    */
   void loadFile();
};

#endif
