#include "Sidebar.hpp"
#include "utils/FileUtils.hpp"
#include <gtk/gtk.h>

namespace Librairie {

Sidebar::Sidebar() {
    // Create scrolled window
    scrolledWindow = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(scrolledWindow, TRUE);
    gtk_widget_set_vexpand(scrolledWindow, TRUE);
    
    // Create tree store
    treeStore = gtk_tree_store_new(NUM_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN);
    
    // Create tree view
    treeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(treeStore));
    
    // Set expand properties using widget functions
    gtk_widget_set_hexpand(treeView, TRUE);
    gtk_widget_set_vexpand(treeView, TRUE);
    
    // Create column for folder names
    GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes(
        "Folders", renderer, "text", COLUMN_NAME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);
    
    // Add tree view to scrolled window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWindow), treeView);
    
    // Connect signals
    g_signal_connect(treeView, "row-activated", 
                     G_CALLBACK(onRowActivated), this);
    
    // Load file system
    loadFileSystem();
}

Sidebar::~Sidebar() {
    // Cleanup will be handled by GTK
}

void Sidebar::loadFileSystem() {
    // Clear existing data
    gtk_tree_store_clear(treeStore);
    
    // Get home directory
    const char* homeDir = g_get_home_dir();
    if (!homeDir) {
        homeDir = "/";
    }
    
    // Create root folder
    rootFolder = std::make_shared<Folder>(homeDir);
    
    // Add root to tree store
    GtkTreeIter rootIter;
    gtk_tree_store_append(treeStore, &rootIter, nullptr);
    gtk_tree_store_set(treeStore, &rootIter,
                       COLUMN_NAME, rootFolder->getDisplayName().c_str(),
                       COLUMN_PATH, rootFolder->path.c_str(),
                       COLUMN_IS_LEAF, FALSE,
                       -1);
    
    // Load root folder contents
    rootFolder->loadContents();
    populateTreeStore(&rootIter, rootFolder);
}

void Sidebar::populateTreeStore(GtkTreeIter* parentIter, const std::shared_ptr<Folder>& folder) {
    // Add subfolders
    for (const auto& subfolder : folder->subfolders) {
        GtkTreeIter iter;
        gtk_tree_store_append(treeStore, &iter, parentIter);
        gtk_tree_store_set(treeStore, &iter,
                           COLUMN_NAME, subfolder->getDisplayName().c_str(),
                           COLUMN_PATH, subfolder->path.c_str(),
                           COLUMN_IS_LEAF, subfolder->subfolders.empty() && subfolder->comics.empty(),
                           -1);
    }
}

void Sidebar::onRowActivated(GtkTreeView* view, GtkTreePath* path, 
                             GtkTreeViewColumn* column, gpointer userData) {
    Sidebar* self = static_cast<Sidebar*>(userData);
    self->handleRowActivated(view, path, column);
}

void Sidebar::handleRowActivated(GtkTreeView* view, GtkTreePath* path, 
                                GtkTreeViewColumn* column) {
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter(GTK_TREE_MODEL(treeStore), &iter, path)) {
        gboolean isLeaf;
        gtk_tree_model_get(GTK_TREE_MODEL(treeStore), &iter, COLUMN_IS_LEAF, &isLeaf, -1);
        
        if (!isLeaf) {
            // Check if this node has children already
            if (!gtk_tree_model_iter_has_child(GTK_TREE_MODEL(treeStore), &iter)) {
                // Get the folder path
                gchar* folderPath = nullptr;
                gtk_tree_model_get(GTK_TREE_MODEL(treeStore), &iter, COLUMN_PATH, &folderPath, -1);
                
                if (folderPath) {
                    // Create folder and load contents
                    auto folder = std::make_shared<Folder>(folderPath);
                    folder->loadContents();
                    
                    // Add children to tree store
                    GtkTreeIter childIter;
                    for (const auto& subfolder : folder->subfolders) {
                        gtk_tree_store_append(treeStore, &childIter, &iter);
                        gtk_tree_store_set(treeStore, &childIter,
                                           COLUMN_NAME, subfolder->getDisplayName().c_str(),
                                           COLUMN_PATH, subfolder->path.c_str(),
                                           COLUMN_IS_LEAF, subfolder->subfolders.empty() && subfolder->comics.empty(),
                                           -1);
                    }
                    
                    g_free(folderPath);
                }
            }
        }
    }
}

} // namespace Librairie
