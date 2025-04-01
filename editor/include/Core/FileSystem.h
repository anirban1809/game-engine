#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <string>

enum ItemType { ITEM_DIRECTORY, ITEM_FILE };

class FileSystemItem {
   public:
    std::string name;
    ItemType type;
};

class FileSystem {
   public:
    std::vector<FileSystemItem> GetItems(const std::string& path);

   private:
};

#endif  // __FILESYSTEM_H__