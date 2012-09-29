#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "Settings.h"

#include <string>
#include <vector>

/** 
 * provides the ability to work with the Falcon phone book file (comHistory.dat).
 * To use this, create a new instance, add one or more phone book entries, and
 * then commit the changes.
 */
class PhoneBook: public Settings
{
public:
   PhoneBook();
   virtual ~PhoneBook();

   /**
    * holds information for a single phone book entry
    */
   class Entry
   {
   public:
      std::string game_name;
      std::string game_ip;
      std::string game_theater;
   };

   /**
    * append the specified entry to this instance; the entry isn't actually
    * appended to the file on disk until you call commitChanges()
    */
   void addEntry(Entry entry);

   /**
    * commit changes, replacing current phone book with the content
    * of this instance, optionally backing up the original file first
    * @param createBackup if true a backup of the phone book file is created first
    * @param merge if true, this merges the entries in this instance with any entries
    *              manually created by the user; if false, the entries in this
    *              instance overwrite the phone book file contents.
    * @param err provides error description on failure
    * @return true if success, false otherwise
    */
   bool commitChanges(bool createBackup, bool merge, std::string & err);

   /**
    * performs initial backup of the phone book file, copying it to the executable
    * directory, just in case
    */
   void doInitialBackup();

private:

   std::vector<Entry> entries;

   /**
    * creates a backup copy of the phone book file
    */
   void makeBackupCopy();

   /**
    * determines path to the phone book file
    */
   bool getPhoneBookFile(std::string & phoneBookFile);
};

#endif
