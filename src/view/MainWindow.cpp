#include "MainWindow.hpp"
#include "Sidebar.hpp"
#include "ComicList.hpp"
#include <gtk/gtk.h>
#include <adwaita.h>

namespace Librairie {

MainWindow::MainWindow(GtkApplication* app) {
    // Create the main window
    window = adw_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Librairie");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
    
    // Create paned layout for split view
    paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_paned_set_position(GTK_PANED(paned), 250);
    gtk_window_set_child(GTK_WINDOW(window), paned);
    
    // Create sidebar (left panel)
    sidebar = new Sidebar();
    gtk_paned_set_start_child(GTK_PANED(paned), sidebar->getWidget());
    
    // Create comic list (right panel)
    comicList = new ComicList();
    gtk_paned_set_end_child(GTK_PANED(paned), comicList->getWidget());
    
    // Connect signals
    GtkSelectionModel* selectionModel = gtk_list_view_get_model(GTK_LIST_VIEW(sidebar->getListView()));
    g_signal_connect(selectionModel, "selection-changed", 
                     G_CALLBACK(onSelectionChanged), this);
    
    // Set resize behavior
    gtk_paned_set_resize_start_child(GTK_PANED(paned), TRUE);
    gtk_paned_set_resize_end_child(GTK_PANED(paned), TRUE);
    gtk_paned_set_shrink_start_child(GTK_PANED(paned), FALSE);
    gtk_paned_set_shrink_end_child(GTK_PANED(paned), FALSE);
}

MainWindow::~MainWindow() {
    delete sidebar;
    delete comicList;
}

void MainWindow::show() {
    gtk_window_present(GTK_WINDOW(window));
}

void MainWindow::onSelectionChanged(GtkSelectionModel* model, guint position, guint n_items, gpointer userData) {
    MainWindow* self = static_cast<MainWindow*>(userData);
    self->handleSelectionChanged(GTK_LIST_VIEW(self->sidebar->getListView()), position);
}

void MainWindow::handleSelectionChanged(GtkListView* view, guint position) {
    GtkSelectionModel* selectionModel = gtk_list_view_get_model(view);
    GtkBitset* selected = gtk_selection_model_get_selection(selectionModel);
    
    if (gtk_bitset_get_size(selected) == 0) return;
    
    guint firstSelected = gtk_bitset_get_first(selected);
    if (firstSelected == G_MAXUINT) return;
    
    // Get the item from the model
    GListModel* listModel = G_LIST_MODEL(gtk_list_view_get_model(view));
    GObject* item = g_list_model_get_item(listModel, firstSelected);
    
    if (item == nullptr) return;
    
    // Get the path from the item (which is a GtkTreeListRow)
    GValue pathValue = G_VALUE_INIT;
    g_object_get_property(G_OBJECT(item), "item", &pathValue);
    
    if (G_VALUE_HOLDS(&pathValue, G_TYPE_STRING)) {
        const gchar* path = g_value_get_string(&pathValue);
        if (path) {
            comicList->loadComicsFromDirectory(path);
        }
    }
    
    g_value_unset(&pathValue);
    g_object_unref(item);
}

} // namespace Librairie
