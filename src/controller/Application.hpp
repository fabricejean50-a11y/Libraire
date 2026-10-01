#pragma once

#include <gtk/gtk.h>
#include <adwaita.h>
#include "../view/MainWindow.hpp"

namespace Librairie {

class Application {
public:
    Application(int argc, char** argv);
    ~Application();
    
    int run();
    
private:
    GtkApplication* app;
    MainWindow* mainWindow;
    
    static void onActivate(GtkApplication* application, gpointer userData);
};

} // namespace Librairie
