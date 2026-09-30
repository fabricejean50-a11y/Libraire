project('comic-library', 'vala',
    version: '0.1.0',
    license: 'GPL-3.0')

# Dependencies pour GNOME 50+ avec GTK4 et libadwaita
gtk_dep = dependency('gtk4', version: '>= 4.0')
adwaita_dep = dependency('libadwaita-1', version: '>= 1.4')
glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')
gio_dep = dependency('gio-2.0')

# Configuration pour Vala
vala_args = [
    '--vapidir', meson.current_source_dir(),
    '--target-glib', '2.72'
]

# Sources Vala
sources = files(
    'src/main.vala',
    'src/window.vala',
    'src/library_view.vala',
    'src/file_tree.vala',
    'src/comic_list.vala'
)

# Génération des fichiers C à partir de Vala
c_sources = valac.compile(sources,
    dependencies: [gtk_dep, adwaita_dep, glib_dep, gobject_dep, gio_dep],
    args: vala_args,
    vala_args: vala_args
)

# Exécutable
executable('comic-library',
    c_sources,
    dependencies: [gtk_dep, adwaita_dep, glib_dep, gobject_dep, gio_dep],
    install: true
)
