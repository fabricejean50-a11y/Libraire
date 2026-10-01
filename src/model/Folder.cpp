#include "Folder.hpp"
#include "utils/FileUtils.hpp"
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

namespace Librairie {

Folder::Folder(const std::string& path, Folder* parent) 
    : path(path), parent(parent) {
    name = FileUtils::getFilename(path);
    if (name.empty()) {
        name = path;
    }
}

void Folder::loadContents() {
    if (loaded) return;
    
    loadSubfolders();
    loadComics();
    loaded = true;
}

void Folder::loadSubfolders() {
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_directory()) {
                auto subfolder = std::make_shared<Folder>(entry.path().string(), this);
                subfolders.push_back(subfolder);
            }
        }
        // Sort subfolders alphabetically
        std::sort(subfolders.begin(), subfolders.end(), 
            [](const auto& a, const auto& b) {
                return a->name < b->name;
            });
    } catch (const fs::filesystem_error& e) {
        g_warning("Error loading subfolders: %s", e.what());
    }
}

void Folder::loadComics() {
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                // Check for comic file extensions
                std::string lowerExt = ext;
                std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), 
                    [](unsigned char c) { return std::tolower(c); });
                
                if (lowerExt == ".cbz" || lowerExt == ".cbr" || lowerExt == ".zip" || lowerExt == ".rar") {
                    auto comic = std::make_shared<Comic>(entry.path().string(), false);
                    if (comic->isValid()) {
                        comics.push_back(comic);
                    }
                }
            }
        }
        // Sort comics alphabetically
        std::sort(comics.begin(), comics.end(), 
            [](const auto& a, const auto& b) {
                return a->name < b->name;
            });
    } catch (const fs::filesystem_error& e) {
        g_warning("Error loading comics: %s", e.what());
    }
}

void Folder::scanForComics() {
    // Recursively scan for comics in this folder and subfolders
    loadComics();
    
    for (auto& subfolder : subfolders) {
        subfolder->scanForComics();
    }
}

std::vector<std::shared_ptr<Comic>> Folder::getAllComics() const {
    std::vector<std::shared_ptr<Comic>> allComics = comics;
    
    for (const auto& subfolder : subfolders) {
        auto subComics = subfolder->getAllComics();
        allComics.insert(allComics.end(), subComics.begin(), subComics.end());
    }
    
    return allComics;
}

std::string Folder::getDisplayName() const {
    return name;
}

std::shared_ptr<Folder> Folder::createRootFolder() {
    // Create root folder pointing to user's home directory
    const char* homeDir = g_get_home_dir();
    return std::make_shared<Folder>(homeDir ? homeDir : "/");
}

} // namespace Librairie
