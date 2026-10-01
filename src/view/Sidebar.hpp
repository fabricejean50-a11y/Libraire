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
    GtkListStore* listStore;
    
    std::shared_ptr<Folder> rootFolder;
    
    // Custom object type for storing folder data
    static GType folder_item_get_type(void);
    
    struct FolderItem {
        GObject parent;
        gchar* name;
        gchar* path;
        gboolean is_leaf;
        guint depth;
    };
    
    void addFolderToStore(GtkListStore* store, const std::shared_ptr<Folder>& folder, guint depth = 0);
    static void onActivate(GtkListView* view, guint position, gpointer userData);
    void handleActivate(GtkListView* view, guint position);
};

} // namespace Librairie
