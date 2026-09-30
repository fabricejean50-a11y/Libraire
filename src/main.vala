using Gtk;
using Adw;
using GLib;

public class ComicLibrary.App : Adw.Application {
    public App() {
        Object(
            application_id: "org.comic_library.App",
            flags: ApplicationFlags.FLAGS_NONE
        );
    }

    protected override void activate() {
        var window = new ComicLibrary.Window(this);
        window.present();
    }

    public static int main(string[] args) {
        var app = new ComicLibrary.App();
        return app.run(args);
    }
}
