#include "controller/Application.hpp"
#include <gtk/gtk.h>

int main(int argc, char** argv) {
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // Create and run the application
    Librairie::Application app(argc, argv);
    return app.run();
}
