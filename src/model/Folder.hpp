#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Comic.hpp"

namespace Librairie {

class Folder {
public:
    Folder(const std::string& path, Folder* parent = nullptr);
    
    // Properties
    std::string path;
    std::string name;
    Folder* parent;
    std::vector<std::shared_ptr<Folder>> subfolders;
    std::vector<std::shared_ptr<Comic>> comics;
    bool loaded = false;
    
    // Methods
    void loadContents();
    void scanForComics();
    std::vector<std::shared_ptr<Comic>> getAllComics() const;
    std::string getDisplayName() const;
    
    // Static methods
    static std::shared_ptr<Folder> createRootFolder();
    
private:
    void loadSubfolders();
    void loadComics();
};

} // namespace Librairie
