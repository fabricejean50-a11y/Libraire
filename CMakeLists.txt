cmake_minimum_required(VERSION 3.10)
project(Librairie LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GTK4 and libadwaita
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(LIBADWAITA REQUIRED libadwaita-1)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)

# Include directories
include_directories(
    ${GTK4_INCLUDE_DIRS}
    ${LIBADWAITA_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIRS}
    ${GOBJECT_INCLUDE_DIRS}
)

# Link directories
link_directories(
    ${GTK4_LIBRARY_DIRS}
    ${LIBADWAITA_LIBRARY_DIRS}
    ${GLIB_LIBRARY_DIRS}
    ${GIO_LIBRARY_DIRS}
    ${GOBJECT_LIBRARY_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/model/Comic.cpp
    src/model/Comic.hpp
    src/model/Folder.cpp
    src/model/Folder.hpp
    src/view/MainWindow.cpp
    src/view/MainWindow.hpp
    src/view/Sidebar.cpp
    src/view/Sidebar.hpp
    src/view/ComicList.cpp
    src/view/ComicList.hpp
    src/controller/Application.cpp
    src/controller/Application.hpp
    src/utils/FileUtils.cpp
    src/utils/FileUtils.hpp
)

# Executable
add_executable(librairie ${SOURCES})

target_link_libraries(librairie
    ${GTK4_LIBRARIES}
    ${LIBADWAITA_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${GIO_LIBRARIES}
    ${GOBJECT_LIBRARIES}
)

target_compile_options(librairie PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -std=c++20
)

# Install
install(TARGETS librairie DESTINATION bin)
