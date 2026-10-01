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
    
    // Connect signals - use GtkTreeView's selection signal directly
    GtkTreeSelection* selection = gtk_tree_view_get_selection(sidebar->getTreeView());
    g_signal_connect(selection, "changed", 
                     G_CALLBACK(onSidebarSelectionChanged), this);
    
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

void MainWindow::onSidebarSelectionChanged(GtkTreeSelection* selection, gpointer userData) {
    MainWindow* self = static_cast<MainWindow*>(userData);
    self->handleSidebarSelectionChanged(selection);
}

void MainWindow::handleSidebarSelectionChanged(GtkTreeSelection* selection) {
    GtkTreeModel* model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar* path = nullptr;
        gtk_tree_model_get(model, &iter, 1, &path, -1); // Column 1 contains the full path
        
        if (path) {
            comicList->loadComicsFromDirectory(path);
            g_free(path);
        }
    }
}

} // namespace Librairie
