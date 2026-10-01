#include "FileUtils.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

namespace Librairie {

std::string FileUtils::getFilename(const std::string& path) {
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        return path.substr(lastSlash + 1);
    }
    return path;
}

std::string FileUtils::getDirectory(const std::string& path) {
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        return path.substr(0, lastSlash);
    }
    return ".";
}

std::string FileUtils::getExtension(const std::string& path) {
    size_t lastDot = path.find_last_of('.');
    if (lastDot != std::string::npos) {
        return path.substr(lastDot);
    }
    return "";
}

std::string FileUtils::removeExtension(const std::string& filename) {
    size_t lastDot = filename.find_last_of('.');
    if (lastDot != std::string::npos) {
        return filename.substr(0, lastDot);
    }
    return filename;
}

bool FileUtils::fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode));
}

bool FileUtils::directoryExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

bool FileUtils::isDirectory(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

std::vector<std::string> FileUtils::getFilesInDirectory(const std::string& path) {
    std::vector<std::string> files;
    
    DIR* dir = opendir(path.c_str());
    if (dir == nullptr) {
        return files;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;
        if (name != "." && name != "..") {
            std::string fullPath = path + "/" + name;
            if (!isDirectory(fullPath)) {
                files.push_back(fullPath);
            }
        }
    }
    
    closedir(dir);
    return files;
}

std::vector<std::string> FileUtils::getDirectoriesInDirectory(const std::string& path) {
    std::vector<std::string> dirs;
    
    DIR* dir = opendir(path.c_str());
    if (dir == nullptr) {
        return dirs;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;
        if (name != "." && name != "..") {
            std::string fullPath = path + "/" + name;
            if (isDirectory(fullPath)) {
                dirs.push_back(fullPath);
            }
        }
    }
    
    closedir(dir);
    return dirs;
}

std::string FileUtils::getFileSizeString(uint64_t size) {
    if (size < 1024) {
        return std::to_string(size) + " B";
    } else if (size < 1024 * 1024) {
        return std::to_string(size / 1024) + " KB";
    } else if (size < 1024 * 1024 * 1024) {
        float mb = size / (1024.0f * 1024.0f);
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.1f MB", mb);
        return std::string(buffer);
    } else {
        float gb = size / (1024.0f * 1024.0f * 1024.0f);
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.1f GB", gb);
        return std::string(buffer);
    }
}

std::string FileUtils::normalizePath(const std::string& path) {
    std::string normalized = path;
    
    // Replace backslashes with forward slashes
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    
    // Remove trailing slash
    if (!normalized.empty() && normalized.back() == '/') {
        normalized.pop_back();
    }
    
    return normalized;
}

} // namespace Librairie
