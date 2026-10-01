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
    
    // Create tree list model with columns: name, path, is_leaf
    GType columnTypes[3] = {G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN};
    treeListModel = gtk_tree_list_model_new(columnTypes, 3, nullptr);
    
    // Create list view with the model
    listView = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_tree_list_model_get_selection(treeListModel)),
                                  nullptr);
    gtk_widget_set_hexpand(listView, TRUE);
    gtk_widget_set_vexpand(listView, TRUE);
    
    // Create factory for list items
    GtkListItemFactory* factory = gtk_signal_list_item_factory_new();
    
    // Setup factory to create widgets for each item
    g_signal_connect(factory, "setup", 
                     G_CALLBACK(+[](GtkSignalListItemFactory* self, GtkListItem* list_item, gpointer user_data) {
                         GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
                         GtkWidget* image = gtk_image_new_from_icon_name("folder-symbolic");
                         GtkWidget* label = gtk_label_new("");
                         
                         gtk_label_set_xalign(GTK_LABEL(label), 0.0);
                         gtk_label_set_ellipsize(GTK_LABEL(label), PANGO_ELLIPSIZE_END);
                         
                         gtk_box_append(GTK_BOX(box), image);
                         gtk_box_append(GTK_BOX(box), label);
                         
                         gtk_list_item_set_child(list_item, box);
                     }), nullptr);
    
    g_signal_connect(factory, "bind", 
                     G_CALLBACK(+[](GtkSignalListItemFactory* self, GtkListItem* list_item, gpointer user_data) {
                         GtkTreeListRow* row = GTK_TREE_LIST_ROW(gtk_list_item_get_item(list_item));
                         GtkWidget* box = gtk_list_item_get_child(list_item);
                         GtkWidget* label = gtk_widget_get_last_child(box);
                         
                         // Get the name and path from the row's data
                         const gchar* name = static_cast<const gchar*>(g_object_get_data(G_OBJECT(row), "folder-name"));
                         if (name) {
                             gtk_label_set_text(GTK_LABEL(label), name);
                         }
                     }), nullptr);
    
    gtk_list_view_set_factory(GTK_LIST_VIEW(listView), factory);
    
    // Add list view to scrolled window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWindow), listView);
    
    // Connect signals
    g_signal_connect(listView, "activate", 
                     G_CALLBACK(onActivate), this);
    
    // Load file system
    loadFileSystem();
}

Sidebar::~Sidebar() {
    // Cleanup will be handled by GTK
}

void Sidebar::loadFileSystem() {
    // Clear existing data
    gtk_tree_list_model_splice(treeListModel, 0, gtk_tree_list_model_get_n_items(treeListModel), nullptr);
    
    // Get home directory
    const char* homeDir = g_get_home_dir();
    if (!homeDir) {
        homeDir = "/";
    }
    
    // Create root folder
    rootFolder = std::make_shared<Folder>(homeDir);
    
    // Create root row with data
    GtkTreeListRow* rootRow = gtk_tree_list_row_new();
    
    // Store folder data as object data
    g_object_set_data_full(G_OBJECT(rootRow), "folder-name", 
                          g_strdup(rootFolder->getDisplayName().c_str()), g_free);
    g_object_set_data_full(G_OBJECT(rootRow), "folder-path", 
                          g_strdup(rootFolder->path.c_str()), g_free);
    g_object_set_data(G_OBJECT(rootRow), "is-leaf", GINT_TO_POINTER(FALSE));
    
    gtk_tree_list_model_append(treeListModel, rootRow);
    
    // Load root folder contents
    rootFolder->loadContents();
    addFolderToModel(rootRow, rootFolder);
}

void Sidebar::addFolderToModel(GtkTreeListRow* parentRow, const std::shared_ptr<Folder>& folder) {
    // Add subfolders
    for (const auto& subfolder : folder->subfolders) {
        GtkTreeListRow* row = gtk_tree_list_row_new();
        
        // Store folder data as object data
        g_object_set_data_full(G_OBJECT(row), "folder-name", 
                              g_strdup(subfolder->getDisplayName().c_str()), g_free);
        g_object_set_data_full(G_OBJECT(row), "folder-path", 
                              g_strdup(subfolder->path.c_str()), g_free);
        g_object_set_data(G_OBJECT(row), "is-leaf", 
                         GINT_TO_POINTER(subfolder->subfolders.empty() && subfolder->comics.empty()));
        
        gtk_tree_list_row_insert_child(parentRow, -1, row);
    }
}

void Sidebar::onActivate(GtkListView* view, guint position, gpointer userData) {
    Sidebar* self = static_cast<Sidebar*>(userData);
    self->handleActivate(view, position);
}

void Sidebar::handleActivate(GtkListView* view, guint position) {
    GtkTreeListModel* model = GTK_TREE_LIST_MODEL(gtk_list_view_get_model(view));
    GtkTreeListRow* row = gtk_tree_list_model_get_row(model, position);
    if (row == nullptr) return;
    
    // Get the is_leaf value from the row's data
    gboolean isLeaf = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(row), "is-leaf"));
    
    if (!isLeaf) {
        // Check if this row has children already
        guint nChildren = gtk_tree_list_row_get_n_children(row);
        if (nChildren == 0) {
            // Get the path from the row's data
            const gchar* folderPath = static_cast<const gchar*>(g_object_get_data(G_OBJECT(row), "folder-path"));
            
            if (folderPath) {
                // Create folder and load contents
                auto folder = std::make_shared<Folder>(folderPath);
                folder->loadContents();
                
                // Add children to model
                for (const auto& subfolder : folder->subfolders) {
                    GtkTreeListRow* childRow = gtk_tree_list_row_new();
                    
                    // Store folder data as object data
                    g_object_set_data_full(G_OBJECT(childRow), "folder-name", 
                                          g_strdup(subfolder->getDisplayName().c_str()), g_free);
                    g_object_set_data_full(G_OBJECT(childRow), "folder-path", 
                                          g_strdup(subfolder->path.c_str()), g_free);
                    g_object_set_data(G_OBJECT(childRow), "is-leaf", 
                                     GINT_TO_POINTER(subfolder->subfolders.empty() && subfolder->comics.empty()));
                    
                    gtk_tree_list_row_insert_child(row, -1, childRow);
                }
            }
        }
    }
}

} // namespace Librairie
