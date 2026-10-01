#pragma once

#include <gtk/gtk.h>
#include <vector>
#include <memory>
#include <string>
#include "../model/Comic.hpp"

namespace Librairie {

class ComicList {
public:
    ComicList();
    ~ComicList();
    
    GtkWidget* getWidget() const { return scrolledWindow; }
    
    void loadComicsFromDirectory(const std::string& directoryPath);
    void setComics(const std::vector<std::shared_ptr<Comic>>& comics);
    
private:
    GtkWidget* scrolledWindow;
    GtkWidget* listBox;
    GtkWidget* emptyLabel;
    
    std::vector<std::shared_ptr<Comic>> currentComics;
    
    void clearList();
    void addComicToList(const std::shared_ptr<Comic>& comic);
    void showEmptyMessage(const std::string& message);
};

} // namespace Librairie
