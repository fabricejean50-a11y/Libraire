# Librairie - Comic Library for GNOME (C++ GTK4)

A modern comic library application for GNOME, built with C++, GTK4, and libadwaita. Inspired by the macOS Librairie app.

## Features

- **Folder Tree Navigation** - Browse your comic collection with a sidebar
- **Comic List Display** - View CBZ/CBR files in the selected directory
- **Modern GNOME UI** - Uses GTK4 and libadwaita for a native look
- **Comic Metadata** - Support for comic metadata (title, series, number, etc.)

## Dependencies

### Debian/Ubuntu
```bash
sudo apt install -y g++ cmake pkg-config libgtk-4-dev libadwaita-1-dev
```

### Fedora
```bash
sudo dnf install -y g++ cmake pkgconf gtk4-devel libadwaita-devel
```

### Arch Linux
```bash
sudo pacman -S g++ cmake pkgconf gtk4 libadwaita
```

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running

```bash
./librairie
```

## Project Structure

```
librairie_cpp/
├── CMakeLists.txt          # CMake build configuration
├── README.md              # This file
└── src/
    ├── main.cpp           # Application entry point
    ├── controller/
    │   ├── Application.hpp
    │   └── Application.cpp # GTK application controller
    ├── model/
    │   ├── Comic.hpp
    │   ├── Comic.cpp       # Comic data model
    │   ├── Folder.hpp
    │   └── Folder.cpp      # Folder/directory model
    ├── utils/
    │   ├── FileUtils.hpp
    │   └── FileUtils.cpp   # File system utilities
    └── view/
        ├── MainWindow.hpp
        ├── MainWindow.cpp   # Main application window
        ├── Sidebar.hpp
        ├── Sidebar.cpp     # Folder tree sidebar
        ├── ComicList.hpp
        └── ComicList.cpp    # Comic list display
```

## Architecture

### Model Layer
- **Comic** - Represents a comic book with metadata
- **Folder** - Represents a directory containing comics or subfolders

### View Layer
- **MainWindow** - Main application window with split view
- **Sidebar** - Tree view for folder navigation
- **ComicList** - List view for displaying comics

### Controller Layer
- **Application** - Main GTK application controller

## Comic File Support

The application currently supports:
- `.cbz` files (ZIP-based comic archives)
- `.cbr` files (RAR-based comic archives)
- `.zip` files
- `.rar` files

## License

GPL-3.0
