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
    
    // Connect signals - use selection-changed on the selection model
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
    self->handleSelectionChanged(GTK_LIST_VIEW(self->sidebar->getListView()));
}

void MainWindow::handleSelectionChanged(GtkListView* view) {
    GtkSelectionModel* selectionModel = gtk_list_view_get_model(view);
    GListModel* listModel = G_LIST_MODEL(gtk_list_view_get_model(view));
    guint nItems = g_list_model_get_n_items(listModel);
    
    // Find first selected position
    guint firstSelected = GTK_INVALID_LIST_POSITION;
    for (guint i = 0; i < nItems; i++) {
        if (gtk_selection_model_is_selected(selectionModel, i)) {
            firstSelected = i;
            break;
        }
    }
    
    if (firstSelected == GTK_INVALID_LIST_POSITION) return;
    
    // Get the item from the model
    gpointer item = g_list_model_get_item(listModel, firstSelected);
    
    if (item == nullptr) return;
    
    // The item is a GtkTreeListRow, get the path data we stored
    const gchar* path = static_cast<const gchar*>(g_object_get_data(G_OBJECT(item), "folder-path"));
    
    if (path) {
        comicList->loadComicsFromDirectory(path);
    }
    
    g_object_unref(G_OBJECT(item));
}

} // namespace Librairie
