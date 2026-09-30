using Gtk;
using GLib;
using Gio;

namespace ComicLibrary {
    public class FileTree : Gtk.ScrolledWindow {
        private Gtk.TreeView tree_view;
        private Gtk.TreeStore tree_store;
        
        // Signal émis lorsqu'un dossier est sélectionné
        public signal void on_directory_selected(string path);

        public FileTree() {
            Object(
                hscrollbar_policy: PolicyType.AUTOMATIC,
                vscrollbar_policy: PolicyType.AUTOMATIC,
                hexpand: true,
                vexpand: true
            );

            create_tree_view();
            load_file_system();
        }

        private void create_tree_view() {
            // Créer le modèle de données pour l'arborescence
            tree_store = new Gtk.TreeStore(typeof(string), typeof(string));
            
            // Créer la TreeView
            tree_view = new Gtk.TreeView.with_model(tree_store);
            tree_view.hexpand = true;
            tree_view.vexpand = true;

            // Ajouter une colonne pour afficher les noms de fichiers
            var column = new Gtk.TreeViewColumn();
            column.title = "Fichiers";
            
            var cell_renderer = new Gtk.CellRendererText();
            column.pack_start(cell_renderer, true);
            column.add_attribute(cell_renderer, "text", 0);
            
            tree_view.append_column(column);

            // Connecter le signal de sélection
            tree_view.get_selection().changed.connect(on_selection_changed);

            // Connecter le signal de double-clic
            tree_view.row_activated.connect(on_row_activated);

            // Ajouter la TreeView au ScrolledWindow
            add(tree_view);
        }

        private void load_file_system() {
            // Commencer par le répertoire personnel de l'utilisateur
            var home_dir = GLib.Environment.get_home_dir();
            
            // Ajouter le répertoire racine
            var root_iter = tree_store.append(null);
            tree_store.set(root_iter, 0, home_dir, 1, home_dir);
            
            // Charger les sous-répertoires
            load_directory_contents(root_iter, home_dir);
        }

        private void load_directory_contents(Gtk.TreeIter parent_iter, string path) {
            try {
                var dir = File.new_for_path(path);
                var enumerator = dir.enumerate_children(
                    FileAttribute.STANDARD_NAME + "," + FileAttribute.STANDARD_TYPE + "," + FileAttribute.STANDARD_DISPLAY_NAME,
                    FileQueryInfoFlags.NONE
                );

                FileInfo info;
                while ((info = enumerator.next_file()) != null) {
                    if (info.get_file_type() == FileType.DIRECTORY) {
                        var display_name = info.get_display_name();
                        var full_path = GLib.Path.build_filename(path, info.get_name());
                        
                        var iter = tree_store.append(parent_iter);
                        tree_store.set(iter, 0, display_name, 1, full_path);
                    }
                }
            } catch (Error e) {
                print("Erreur lors du chargement du répertoire %s: %s\n", path, e.message);
            }
        }

        private void on_selection_changed(Gtk.TreeSelection selection) {
            Gtk.TreeIter iter;
            Gtk.TreeModel model;
            
            if (selection.get_selected(out model, out iter)) {
                GLib.Value value;
                tree_store.get_value(iter, 1, out value);
                var path = (string)value;
                
                // Émettre le signal avec le chemin sélectionné
                on_directory_selected(path);
            }
        }

        private void on_row_activated(Gtk.TreeView view, Gtk.TreePath path, Gtk.TreeViewColumn column) {
            Gtk.TreeIter iter;
            if (tree_store.get_iter(out iter, path)) {
                GLib.Value value;
                tree_store.get_value(iter, 1, out value);
                var selected_path = (string)value;
                
                // Vérifier si ce dossier a déjà des enfants chargés
                if (!tree_store.iter_has_child(iter)) {
                    // Charger les sous-répertoires
                    load_directory_contents(iter, selected_path);
                }
                
                // Émettre le signal
                on_directory_selected(selected_path);
            }
        }
    }
}
