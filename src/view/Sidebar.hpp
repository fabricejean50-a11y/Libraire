#pragma once

#include <gtk/gtk.h>
#include <vector>
#include <memory>
#include <string>
#include "../model/Folder.hpp"

namespace Librairie {

class Sidebar {
public:
    Sidebar();
    ~Sidebar();
    
    GtkWidget* getWidget() const { return scrolledWindow; }
    GtkWidget* getListView() const { return listView; }
    
    void loadFileSystem();
    
private:
    GtkWidget* scrolledWindow;
    GtkWidget* listView;
    GtkTreeListModel* treeListModel;
    GtkTreeListRow* rootRow;
    
    std::shared_ptr<Folder> rootFolder;
    
    // Store folder data for each row
    struct RowData {
        std::string path;
        std::string name;
        bool isLeaf;
    };
    
    void addFolderToModel(GtkTreeListRow* parentRow, const std::shared_ptr<Folder>& folder);
    static void onActivate(GtkListView* view, guint position, gpointer userData);
    void handleActivate(GtkListView* view, guint position);
};

} // namespace Librairie
