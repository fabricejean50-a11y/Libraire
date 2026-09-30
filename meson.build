project('comic-library', 'vala',
    version: '0.1.0',
    license: 'GPL-3.0')

# Dependencies pour GNOME 50+ avec GTK4 et libadwaita
gtk_dep = dependency('gtk4', version: '>= 4.0')
adwaita_dep = dependency('libadwaita-1', version: '>= 1.4')
glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')
gio_dep = dependency('gio-2.0')

# Chemins des VAPI pour Vala
vala_vapi_dirs = [
    '/usr/share/vala/vapi',
    '/usr/lib/vala/vapi',
    '/usr/lib/x86_64-linux-gnu/vala/vapi'
]

# Configuration pour Vala
vala_args = [
    '--target-glib', '2.72'
]

# Ajouter les chemins VAPI si ils existent
foreach vapi_dir : vala_vapi_dirs
    if os.path.exists(vapi_dir)
        vala_args += ['--vapidir', vapi_dir]
    endif
endforeach

# Sources Vala
sources = files(
    'src/main.vala',
    'src/window.vala',
    'src/library_view.vala',
    'src/file_tree.vala',
    'src/comic_list.vala'
)

# Exécutable
executable('comic-library',
    sources,
    dependencies: [gtk_dep, adwaita_dep, glib_dep, gobject_dep, gio_dep],
    vala_args: vala_args,
    install: true
)
