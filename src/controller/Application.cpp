#include "Application.hpp"
#include "../view/MainWindow.hpp"
#include <gtk/gtk.h>
#include <adwaita.h>

namespace Librairie {

Application::Application(int argc, char** argv) {
    // Initialize libadwaita
    adw_init(&argc, &argv);
    
    // Create GTK application
    app = gtk_application_new("org.librairie.App", G_APPLICATION_DEFAULT_FLAGS);
    
    // Connect activate signal
    g_signal_connect(app, "activate", G_CALLBACK(onActivate), this);
}

Application::~Application() {
    if (mainWindow) {
        delete mainWindow;
    }
    if (app) {
        g_object_unref(app);
    }
}

void Application::onActivate(GtkApplication* application, gpointer userData) {
    Application* self = static_cast<Application*>(userData);
    self->mainWindow = new MainWindow(application);
    self->mainWindow->show();
}

int Application::run() {
    // Run the application
    int status = g_application_run(G_APPLICATION(app), 0, nullptr);
    
    return status;
}

} // namespace Librairie
