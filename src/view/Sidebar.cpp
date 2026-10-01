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
                         
                         // Get the name value from the row's item
                         GObject* item = gtk_tree_list_row_get_item(row);
                         if (item != nullptr) {
                             GValue nameValue = G_VALUE_INIT;
                             g_value_init(&nameValue, G_TYPE_STRING);
                             g_object_get_property(item, "item", &nameValue);
                             
                             if (G_VALUE_HOLDS(&nameValue, G_TYPE_STRING)) {
                                 gtk_label_set_text(GTK_LABEL(label), g_value_get_string(&nameValue));
                             }
                             
                             g_value_unset(&nameValue);
                             g_object_unref(item);
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
    GValue rootValues[3];
    rootValues[0] = G_VALUE_INIT; // name
    rootValues[1] = G_VALUE_INIT; // path
    rootValues[2] = G_VALUE_INIT; // is_leaf
    
    g_value_init(&rootValues[0], G_TYPE_STRING);
    g_value_init(&rootValues[1], G_TYPE_STRING);
    g_value_init(&rootValues[2], G_TYPE_BOOLEAN);
    
    g_value_set_string(&rootValues[0], rootFolder->getDisplayName().c_str());
    g_value_set_string(&rootValues[1], rootFolder->path.c_str());
    g_value_set_boolean(&rootValues[2], FALSE);
    
    GtkTreeListRow* rootRow = gtk_tree_list_row_new();
    gtk_tree_list_row_set_values(rootRow, 3, rootValues);
    
    gtk_tree_list_model_append(treeListModel, rootRow);
    
    // Load root folder contents
    rootFolder->loadContents();
    addFolderToModel(rootRow, rootFolder);
    
    // Cleanup values
    for (int i = 0; i < 3; i++) {
        g_value_unset(&rootValues[i]);
    }
}

void Sidebar::addFolderToModel(GtkTreeListRow* parentRow, const std::shared_ptr<Folder>& folder) {
    // Add subfolders
    for (const auto& subfolder : folder->subfolders) {
        GValue values[3];
        values[0] = G_VALUE_INIT; // name
        values[1] = G_VALUE_INIT; // path
        values[2] = G_VALUE_INIT; // is_leaf
        
        g_value_init(&values[0], G_TYPE_STRING);
        g_value_init(&values[1], G_TYPE_STRING);
        g_value_init(&values[2], G_TYPE_BOOLEAN);
        
        g_value_set_string(&values[0], subfolder->getDisplayName().c_str());
        g_value_set_string(&values[1], subfolder->path.c_str());
        g_value_set_boolean(&values[2], subfolder->subfolders.empty() && subfolder->comics.empty());
        
        GtkTreeListRow* row = gtk_tree_list_row_new();
        gtk_tree_list_row_set_values(row, 3, values);
        
        gtk_tree_list_row_insert_child(parentRow, -1, row);
        
        // Cleanup values
        for (int i = 0; i < 3; i++) {
            g_value_unset(&values[i]);
        }
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
    
    // Get the is_leaf value from the row's item
    GObject* item = gtk_tree_list_row_get_item(row);
    if (item == nullptr) return;
    
    GValue isLeafValue = G_VALUE_INIT;
    g_value_init(&isLeafValue, G_TYPE_BOOLEAN);
    g_object_get_property(item, "item", &isLeafValue);
    
    gboolean isLeaf = FALSE;
    if (G_VALUE_HOLDS(&isLeafValue, G_TYPE_BOOLEAN)) {
        isLeaf = g_value_get_boolean(&isLeafValue);
    }
    g_value_unset(&isLeafValue);
    g_object_unref(item);
    
    if (!isLeaf) {
        // Check if this row has children already
        guint nChildren = gtk_tree_list_row_get_n_children(row);
        if (nChildren == 0) {
            // Get the path value from the row's item
            GObject* pathItem = gtk_tree_list_row_get_item(row);
            if (pathItem == nullptr) return;
            
            GValue pathValue = G_VALUE_INIT;
            g_value_init(&pathValue, G_TYPE_STRING);
            g_object_get_property(pathItem, "item", &pathValue);
            
            const gchar* folderPath = nullptr;
            if (G_VALUE_HOLDS(&pathValue, G_TYPE_STRING)) {
                folderPath = g_value_get_string(&pathValue);
            }
            g_value_unset(&pathValue);
            g_object_unref(pathItem);
            
            if (folderPath) {
                // Create folder and load contents
                auto folder = std::make_shared<Folder>(folderPath);
                folder->loadContents();
                
                // Add children to model
                for (const auto& subfolder : folder->subfolders) {
                    GValue childValues[3];
                    childValues[0] = G_VALUE_INIT;
                    childValues[1] = G_VALUE_INIT;
                    childValues[2] = G_VALUE_INIT;
                    
                    g_value_init(&childValues[0], G_TYPE_STRING);
                    g_value_init(&childValues[1], G_TYPE_STRING);
                    g_value_init(&childValues[2], G_TYPE_BOOLEAN);
                    
                    g_value_set_string(&childValues[0], subfolder->getDisplayName().c_str());
                    g_value_set_string(&childValues[1], subfolder->path.c_str());
                    g_value_set_boolean(&childValues[2], subfolder->subfolders.empty() && subfolder->comics.empty());
                    
                    GtkTreeListRow* childRow = gtk_tree_list_row_new();
                    gtk_tree_list_row_set_values(childRow, 3, childValues);
                    
                    gtk_tree_list_row_insert_child(row, -1, childRow);
                    
                    // Cleanup values
                    for (int i = 0; i < 3; i++) {
                        g_value_unset(&childValues[i]);
                    }
                }
            }
        }
    }
}

} // namespace Librairie
