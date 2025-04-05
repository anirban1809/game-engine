#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <Core/Types.h>
#include <string>

enum ItemType { ITEM_DIRECTORY, ITEM_FILE };

class FileSystemItem {
   public:
    std::string name;
    ItemType type;
    uint32 size;
    std::string extension;
};

class FileSystem {
   public:
    std::vector<FileSystemItem> GetItems(const std::string& path);
    void CreateNewFolder(const std::string& path, std::string& name);

   private:
};

#endif  // __FILESYSTEM_H__