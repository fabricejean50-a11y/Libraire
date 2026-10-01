#pragma once

#include <gtk/gtk.h>
#include <vector>
#include <memory>
#include "../model/Folder.hpp"

namespace Librairie {

class Sidebar {
public:
    Sidebar();
    ~Sidebar();
    
    GtkWidget* getWidget() const { return scrolledWindow; }
    GtkWidget* getTreeView() const { return treeView; }
    
    void loadFileSystem();
    
private:
    GtkWidget* scrolledWindow;
    GtkWidget* treeView;
    GtkTreeStore* treeStore;
    
    std::shared_ptr<Folder> rootFolder;
    
    // Tree model columns
    enum {
        COLUMN_NAME = 0,
        COLUMN_PATH,
        COLUMN_IS_LEAF,
        NUM_COLUMNS
    };
    
    void populateTreeStore(GtkTreeIter* parentIter, const std::shared_ptr<Folder>& folder);
    static void onRowActivated(GtkTreeView* view, GtkTreePath* path, 
                              GtkTreeViewColumn* column, gpointer userData);
    void handleRowActivated(GtkTreeView* view, GtkTreePath* path, 
                           GtkTreeViewColumn* column);
};

} // namespace Librairie
