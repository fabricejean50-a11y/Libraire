#include "Sidebar.hpp"
#include "utils/FileUtils.hpp"
#include <gtk/gtk.h>

namespace Librairie {

// Custom GObject type for folder items
G_DEFINE_TYPE(Sidebar_FolderItem, Sidebar_FolderItem, G_TYPE_OBJECT)

struct _Sidebar_FolderItem {
    GObject parent;
    gchar* name;
    gchar* path;
    gboolean is_leaf;
    guint depth;
};

struct _Sidebar_FolderItemClass {
    GObjectClass parent_class;
};

static void
Sidebar_FolderItem_finalize(GObject *obj)
{
    Sidebar_FolderItem *item = (Sidebar_FolderItem *)obj;
    g_free(item->name);
    g_free(item->path);
    G_OBJECT_CLASS(Sidebar_FolderItem_parent_class)->finalize(obj);
}

static void
Sidebar_FolderItem_class_init(Sidebar_FolderItemClass *klass)
{
    G_OBJECT_CLASS(klass)->finalize = Sidebar_FolderItem_finalize;
}

static void
Sidebar_FolderItem_init(Sidebar_FolderItem *item)
{
    item->name = nullptr;
    item->path = nullptr;
    item->is_leaf = FALSE;
    item->depth = 0;
}

GType
Sidebar::folder_item_get_type(void)
{
    return Sidebar_FolderItem_get_type();
}

Sidebar::Sidebar() {
    // Create scrolled window
    scrolledWindow = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(scrolledWindow, TRUE);
    gtk_widget_set_vexpand(scrolledWindow, TRUE);
    
    // Create list store with our custom type
    listStore = gtk_list_store_new(folder_item_get_type());
    
    // Create list view with the model
    listView = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_list_store_get_selection(listStore)),
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
                         GObject* item = gtk_list_item_get_item(list_item);
                         Sidebar_FolderItem* folderItem = (Sidebar_FolderItem*)item;
                         GtkWidget* box = gtk_list_item_get_child(list_item);
                         GtkWidget* label = gtk_widget_get_last_child(box);
                         
                         if (folderItem->name) {
                             gtk_label_set_text(GTK_LABEL(label), folderItem->name);
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
    gtk_list_store_splice(listStore, 0, gtk_list_store_get_n_items(listStore), nullptr);
    
    // Get home directory
    const char* homeDir = g_get_home_dir();
    if (!homeDir) {
        homeDir = "/";
    }
    
    // Create root folder
    rootFolder = std::make_shared<Folder>(homeDir);
    
    // Add root folder to store
    addFolderToStore(listStore, rootFolder, 0);
}

void Sidebar::addFolderToStore(GtkListStore* store, const std::shared_ptr<Folder>& folder, guint depth) {
    // Create folder item
    Sidebar_FolderItem* item = (Sidebar_FolderItem*)g_object_new(folder_item_get_type(), nullptr);
    item->name = g_strdup(folder->getDisplayName().c_str());
    item->path = g_strdup(folder->path.c_str());
    item->is_leaf = folder->subfolders.empty() && folder->comics.empty();
    item->depth = depth;
    
    // Add to store
    gtk_list_store_append(store, G_OBJECT(item));
    g_object_unref(item);
    
    // If this is not a leaf and we're at depth 0 or 1, load subfolders
    if (!item->is_leaf && depth < 2) {
        for (const auto& subfolder : folder->subfolders) {
            addFolderToStore(store, subfolder, depth + 1);
        }
    }
}

void Sidebar::onActivate(GtkListView* view, guint position, gpointer userData) {
    Sidebar* self = static_cast<Sidebar*>(userData);
    self->handleActivate(view, position);
}

void Sidebar::handleActivate(GtkListView* view, guint position) {
    GObject* itemObj = g_list_model_get_object(G_LIST_MODEL(listStore), position);
    if (itemObj == nullptr) return;
    
    Sidebar_FolderItem* folderItem = (Sidebar_FolderItem*)itemObj;
    
    if (!folderItem->is_leaf) {
        // Check if we need to load subfolders
        // For now, just reload the entire filesystem for simplicity
        loadFileSystem();
    }
    
    g_object_unref(itemObj);
}

} // namespace Librairie

// Define the GType implementation
G_DEFINE_TYPE(Sidebar_FolderItem, Sidebar_FolderItem, G_TYPE_OBJECT)
