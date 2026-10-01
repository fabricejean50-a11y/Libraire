#pragma once

#include <string>
#include <vector>
#include <sys/stat.h>

namespace Librairie {

class FileUtils {
public:
    static std::string getFilename(const std::string& path);
    static std::string getDirectory(const std::string& path);
    static std::string getExtension(const std::string& path);
    static std::string removeExtension(const std::string& filename);
    static bool fileExists(const std::string& path);
    static bool directoryExists(const std::string& path);
    static bool isDirectory(const std::string& path);
    static std::vector<std::string> getFilesInDirectory(const std::string& path);
    static std::vector<std::string> getDirectoriesInDirectory(const std::string& path);
    static std::string getFileSizeString(uint64_t size);
    static std::string normalizePath(const std::string& path);
    
private:
    FileUtils() = delete;
    ~FileUtils() = delete;
};

} // namespace Librairie
