#pragma once

#include <gtk/gtk.h>
#include <adwaita.h>
#include "Sidebar.hpp"
#include "ComicList.hpp"

namespace Librairie {

class MainWindow {
public:
    MainWindow(GtkApplication* app);
    ~MainWindow();
    
    GtkWindow* getWindow() const { return GTK_WINDOW(window); }
    void show();
    
private:
    GtkWidget* window;
    GtkWidget* paned;
    Sidebar* sidebar;
    ComicList* comicList;
    
    // Signal handlers
    static void onSidebarSelectionChanged(GtkTreeSelection* selection, gpointer userData);
    static void onActivate(GtkApplication* app, gpointer userData);
    
    void handleSidebarSelectionChanged(GtkTreeSelection* selection);
};

} // namespace Librairie
