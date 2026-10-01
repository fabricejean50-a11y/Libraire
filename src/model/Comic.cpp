#include "Comic.hpp"
#include "utils/FileUtils.hpp"
#include <algorithm>
#include <cctype>
#include <sys/stat.h>

namespace Librairie {

Comic::Comic(const std::string& fullpath, bool addPageList) : fullpath(fullpath) {
    name = FileUtils::getFilename(fullpath);
    if (addPageList) {
        // Load page list from CBZ
        // This will be implemented later for comic reading
    }
    extractMetadataFromPath();
}

void Comic::extractMetadataFromPath() {
    // Extract basic metadata from filename/path
    title = name;
    
    // Remove extension
    size_t lastDot = name.find_last_of('.');
    if (lastDot != std::string::npos) {
        title = name.substr(0, lastDot);
    }
    
    // Try to extract number from filename (e.g., "Comic #001.cbz" -> number = 1)
    size_t hashPos = title.find('#');
    if (hashPos != std::string::npos) {
        size_t endPos = title.find_first_not_of("0123456789", hashPos + 1);
        if (endPos != std::string::npos) {
            std::string numStr = title.substr(hashPos + 1, endPos - hashPos - 1);
            try {
                number = std::stoi(numStr);
            } catch (...) {
                // Ignore conversion errors
            }
        }
    }
    
    // Set default cover file
    coverfile = fullpath; // Will be updated when reading the comic
}

void Comic::loadMetadata() {
    // Load metadata from comic file
    // Implementation for CBZ reading will be added later
}

bool Comic::isValid() const {
    return !fullpath.empty() && FileUtils::fileExists(fullpath);
}

std::string Comic::getDisplayName() const {
    if (!serie.empty() && number > 0) {
        return serie + " #" + std::to_string(number);
    }
    return title;
}

} // namespace Librairie
