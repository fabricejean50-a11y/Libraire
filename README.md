# Comic Library - Gestionnaire de BD et Manga

Une application GNOME moderne pour gérer et lire vos collections de BD et Manga au format CBZ.

## Prérequis

- GNOME 50+ ou GTK 4.0+
- libadwaita 1.4+
- Vala compiler (valac)
- Meson build system
- pkg-config

### Installation des dépendances (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install -y valac meson libgtk-4-dev libadwaita-1-dev pkg-config
```

### Installation des dépendances (Fedora)

```bash
sudo dnf install -y vala meson gtk4-devel libadwaita-devel pkg-config
```

### Installation des dépendances (Arch Linux)

```bash
sudo pacman -S vala meson gtk4 libadwaita pkgconf
```

## Compilation

```bash
cd comic_library
meson setup builddir --prefix=/usr/local
cd builddir
ninja
```

## Installation

```bash
sudo ninja install
```

## Exécution

```bash
comic-library
```

Ou depuis le répertoire de build :

```bash
./comic-library
```

## Fonctionnalités

### Partie 1: Gestion de bibliothèque (implémentée)
- **Arborescence des dossiers** : Navigation dans vos répertoires
- **Liste des fichiers CBZ** : Affichage des fichiers CBZ dans le dossier sélectionné
- **Affichage des métadonnées** : Taille des fichiers
- **Interface moderne** : Utilisation de libadwaita pour une intégration parfaite avec GNOME

### Partie 2: Lecture (à implémenter)
- Ouverture des fichiers CBZ
- Visualisation des images
- Navigation entre les pages
- Zoom et rotation

## Structure du projet

```
comic_library/
├── meson.build          # Configuration de compilation
├── README.md           # Documentation
└── src/
    ├── main.vala       # Point d'entrée de l'application
    ├── window.vala     # Fenêtre principale
    ├── file_tree.vala  # Arborescence des fichiers
    ├── comic_list.vala # Liste des fichiers CBZ
    └── library_view.vala # Vue de la bibliothèque
```

## Architecture

L'application utilise une architecture split-view :
- **Panneau gauche** : `FileTree` - Arborescence des dossiers navigable
- **Panneau droit** : `ComicList` - Liste des fichiers CBZ dans le dossier sélectionné

## Personnalisation

Vous pouvez modifier le comportement de l'application en ajustant :
- La position du split view dans `window.vala`
- Les extensions de fichiers reconnues dans `comic_list.vala`
- Le style CSS dans `comic_list.vala`

## Contribution

Les contributions sont les bienvenues ! Ouvrez un PR ou un issue pour :
- Signaler des bugs
- Proposer des fonctionnalités
- Améliorer le code

## Licence

GPL-3.0
