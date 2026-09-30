using Gtk;
using Adw;
using GLib;
using Gio;

namespace ComicLibrary {
    public class Window : Adw.ApplicationWindow {
        private Gtk.Box root_box;
        private FileTree file_tree;
        private ComicList comic_list;

        public Window(Adw.Application app) {
            Object(
                application: app,
                title: "Comic Library",
                default_width: 1200,
                default_height: 800,
                content: create_main_layout()
            );
        }

        private Widget create_main_layout() {
            // Créer un split view pour séparer l'arborescence et la liste
            var split_view = new Gtk.Paned(Orientation.HORIZONTAL);
            split_view.position = 250;

            // Créer le panneau gauche (arborescence)
            file_tree = new FileTree();
            split_view.start_child = file_tree;

            // Créer le panneau droit (liste des CBZ)
            comic_list = new ComicList();
            split_view.end_child = comic_list;

            // Connecter les signaux entre les deux parties
            file_tree.on_directory_selected.connect((path) => {
                comic_list.load_comics_from_directory(path);
            });

            // Retourner le split view comme widget racine
            return split_view;
        }
    }
}
