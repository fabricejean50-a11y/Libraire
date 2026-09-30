using Gtk;
using Adw;
using GLib;

namespace ComicLibrary {
    public class LibraryView : Gtk.Box {
        public LibraryView() {
            Object(
                orientation: Orientation.VERTICAL,
                spacing: 12,
                margin_top: 12,
                margin_bottom: 12,
                margin_start: 12,
                margin_end: 12
            );
        }
    }
}
