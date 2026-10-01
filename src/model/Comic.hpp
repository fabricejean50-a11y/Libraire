#pragma once

#include <string>
#include <vector>
#include <glib.h>

namespace Librairie {

class Comic {
public:
    Comic(const std::string& fullpath, bool addPageList = false);
    
    // Properties
    std::string fullpath;
    std::string name;
    std::string coverfile;
    std::string title;
    std::string serie;
    std::string genre;
    std::string writer;
    std::string penciler;
    std::string color;
    std::string editor;
    std::string synopsis;
    std::string comicID;
    int pageCount = 0;
    int number = 0;
    int ofnumber = 0;
    int date = 0;
    bool manga = false;
    std::vector<std::string> pageList;
    
    // Methods
    void loadMetadata();
    bool isValid() const;
    std::string getDisplayName() const;
    
private:
    void extractMetadataFromPath();
};

} // namespace Librairie
