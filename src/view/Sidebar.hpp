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
    
    // Column indices for the tree list model
    enum Column {
        COLUMN_NAME,
        COLUMN_PATH,
        COLUMN_IS_LEAF,
        NUM_COLUMNS
    };
    
    // Store folder data for each row
    struct FolderData {
        std::shared_ptr<Folder> folder;
        bool loaded = false;
    };
    
    void addFolderToModel(GtkTreeListRow* parentRow, const std::shared_ptr<Folder>& folder);
    static void onActivate(GtkListView* view, guint position, gpointer userData);
    void handleActivate(GtkListView* view, guint position);
};

} // namespace Librairie
