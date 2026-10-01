#include "controller/Application.hpp"

int main(int argc, char** argv) {
    // Create and run the application
    // GTK4 initialization is handled automatically by GtkApplication
    Librairie::Application app(argc, argv);
    return app.run();
}
