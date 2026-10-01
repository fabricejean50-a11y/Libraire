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
    g_signal_connect(sidebar->getListView(), "activate", 
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

void MainWindow::onSelectionChanged(GtkListView* view, guint position, gpointer userData) {
    MainWindow* self = static_cast<MainWindow*>(userData);
    self->handleSelectionChanged(view, position);
}

void MainWindow::handleSelectionChanged(GtkListView* view, guint position) {
    GtkTreeListRow* row = gtk_list_view_get_row_at_pos(view, position);
    if (row == nullptr) return;
    
    // Get the tree list model from the sidebar
    GtkTreeListModel* model = GTK_TREE_LIST_MODEL(gtk_list_view_get_model(view));
    if (model == nullptr) return;
    
    // Get the path from the row
    g_autoptr(GValue) pathValue = gtk_tree_list_model_get_value(model, row, 1); // COLUMN_PATH
    const gchar* path = g_value_get_string(pathValue);
    
    if (path) {
        comicList->loadComicsFromDirectory(path);
    }
}

} // namespace Librairie
