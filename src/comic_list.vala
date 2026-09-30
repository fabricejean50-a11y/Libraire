using Gtk;
using GLib;
using Gio;

namespace ComicLibrary {
    public class ComicList : Gtk.ScrolledWindow {
        private Gtk.ListBox list_box;
        private Gtk.Label empty_label;
        private string current_directory;

        public ComicList() {
            Object(
                hscrollbar_policy: PolicyType.AUTOMATIC,
                vscrollbar_policy: PolicyType.AUTOMATIC,
                hexpand: true,
                vexpand: true
            );

            create_list_box();
            show_empty_message();
        }

        private void create_list_box() {
            list_box = new Gtk.ListBox();
            list_box.hexpand = true;
            list_box.vexpand = true;
            list_box.set_selection_mode(Gtk.SelectionMode.SINGLE);
            
            // Ajouter un style CSS pour les éléments
            var css_provider = new Gtk.CssProvider();
            css_provider.load_from_data("""
                list row {
                    padding: 8px;
                    border-radius: 4px;
                }
                list row:hover {
                    background-color: @theme_base_color;
                }
                list row:selected {
                    background-color: @theme_selected_bg_color;
                }
            """.data);
            
            var style_context = list_box.get_style_context();
            style_context.add_provider(css_provider, Gtk.STYLE_PROVIDER_PRIORITY_APPLICATION);

            add(list_box);
        }

        private void show_empty_message() {
            empty_label = new Gtk.Label("Sélectionnez un dossier pour afficher les fichiers CBZ");
            empty_label.halign = Gtk.Align.CENTER;
            empty_label.valign = Gtk.Align.CENTER;
            empty_label.margin = 24;
            empty_label.get_style_context().add_class("dim-label");
            
            // Retirer l'ancien message si nécessaire
            if (list_box.get_children().length() > 0) {
                foreach (var child in list_box.get_children()) {
                    list_box.remove(child);
                }
            }
            
            list_box.append(empty_label);
        }

        public void load_comics_from_directory(string directory_path) {
            current_directory = directory_path;
            
            // Vider la liste actuelle
            foreach (var child in list_box.get_children()) {
                list_box.remove(child);
            }

            try {
                var dir = File.new_for_path(directory_path);
                var enumerator = dir.enumerate_children(
                    FileAttribute.STANDARD_NAME + "," + FileAttribute.STANDARD_TYPE + "," + FileAttribute.STANDARD_SIZE,
                    FileQueryInfoFlags.NONE
                );

                FileInfo info;
                bool has_comics = false;
                
                while ((info = enumerator.next_file()) != null) {
                    var file_name = info.get_name();
                    var file_type = info.get_file_type();
                    
                    // Vérifier si c'est un fichier CBZ (extension .cbz)
                    if (file_type == FileType.REGULAR && 
                        file_name.down().has_suffix(".cbz")) {
                        has_comics = true;
                        create_comic_row(file_name, info.get_size());
                    }
                }

                if (!has_comics) {
                    show_empty_message();
                    empty_label.set_text("Aucun fichier CBZ trouvé dans ce dossier");
                }
                
            } catch (Error e) {
                print("Erreur lors du chargement des fichiers CBZ: %s\n", e.message);
                show_empty_message();
                empty_label.set_text("Erreur lors du chargement: " + e.message);
            }
        }

        private void create_comic_row(string file_name, int64 file_size) {
            var row = new Gtk.ListBoxRow();
            row.margin = 4;
            
            var box = new Gtk.Box(Orientation.HORIZONTAL, 12);
            box.margin = 8;
            
            // Icône pour le fichier CBZ
            var icon = new Gtk.Image.from_icon_name("application-zip");
            icon.pixel_size = 32;
            box.append(icon);
            
            // Informations sur le fichier
            var info_box = new Gtk.Box(Orientation.VERTICAL, 4);
            
            var title_label = new Gtk.Label(file_name);
            title_label.halign = Gtk.Align.START;
            title_label.ellipsize = Pango.EllipsizeMode.END;
            title_label.max_width_chars = 40;
            title_label.get_style_context().add_class("title-4");
            
            var size_label = new Gtk.Label(format_size(file_size));
            size_label.halign = Gtk.Align.START;
            size_label.get_style_context().add_class("dim-label");
            
            info_box.append(title_label);
            info_box.append(size_label);
            box.append(info_box);
            
            // Espace réservé pour une couverture (à implémenter plus tard)
            var cover_box = new Gtk.Box(Orientation.HORIZONTAL, 0);
            cover_box.width_request = 64;
            box.append(cover_box);
            
            row.set_child(box);
            list_box.append(row);
        }

        private string format_size(int64 bytes) {
            if (bytes < 1024) {
                return "%d octets".printf(bytes);
            } else if (bytes < 1024 * 1024) {
                return "%.1f Ko".printf(bytes / 1024.0);
            } else if (bytes < 1024 * 1024 * 1024) {
                return "%.1f Mo".printf(bytes / (1024.0 * 1024.0));
            } else {
                return "%.1f Go".printf(bytes / (1024.0 * 1024.0 * 1024.0));
            }
        }
    }
}
