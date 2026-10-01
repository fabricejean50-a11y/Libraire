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
    static void onSelectionChanged(GtkListView* view, guint position, gpointer userData);
    void handleSelectionChanged(GtkListView* view, guint position);
};

} // namespace Librairie
