#include "ComicList.hpp"
#include "FileUtils.hpp"
#include <gtk/gtk.h>

namespace Librairie {

ComicList::ComicList() {
    // Create scrolled window
    scrolledWindow = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(scrolledWindow, TRUE);
    gtk_widget_set_vexpand(scrolledWindow, TRUE);
    
    // Create list box
    listBox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(listBox), GTK_SELECTION_SINGLE);
    gtk_widget_set_hexpand(listBox, TRUE);
    gtk_widget_set_vexpand(listBox, TRUE);
    
    // Add list box to scrolled window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWindow), listBox);
    
    // Create empty message
    emptyLabel = gtk_label_new("Select a folder to display comics");
    gtk_label_set_xalign(GTK_LABEL(emptyLabel), 0.5);
    gtk_label_set_yalign(GTK_LABEL(emptyLabel), 0.5);
    gtk_widget_set_margin_top(emptyLabel, 24);
    gtk_widget_set_margin_bottom(emptyLabel, 24);
    gtk_widget_set_margin_start(emptyLabel, 24);
    gtk_widget_set_margin_end(emptyLabel, 24);
    
    // Add CSS class for styling
    gtk_widget_add_css_class(emptyLabel, "dim-label");
    
    // Show empty message initially
    showEmptyMessage("Select a folder to display comics");
}

ComicList::~ComicList() {
    // Cleanup will be handled by GTK
}

void ComicList::loadComicsFromDirectory(const std::string& directoryPath) {
    currentComics.clear();
    clearList();
    
    if (!FileUtils::directoryExists(directoryPath)) {
        showEmptyMessage("Directory does not exist");
        return;
    }
    
    // Scan directory for comics
    auto folder = std::make_shared<Folder>(directoryPath);
    folder->loadContents();
    
    if (folder->comics.empty()) {
        showEmptyMessage("No comics found in this directory");
    } else {
        setComics(folder->comics);
    }
}

void ComicList::setComics(const std::vector<std::shared_ptr<Comic>>& comics) {
    currentComics = comics;
    clearList();
    
    for (const auto& comic : comics) {
        addComicToList(comic);
    }
}

void ComicList::clearList() {
    // Remove all children from list box
    GtkWidget* child;
    while ((child = gtk_widget_get_first_child(listBox)) != NULL) {
        gtk_list_box_remove(GTK_LIST_BOX(listBox), child);
    }
}

void ComicList::addComicToList(const std::shared_ptr<Comic>& comic) {
    // Create a row
    GtkWidget* row = gtk_list_box_row_new();
    gtk_widget_set_margin_top(row, 4);
    gtk_widget_set_margin_bottom(row, 4);
    
    // Create a horizontal box for the row content
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_start(box, 8);
    gtk_widget_set_margin_end(box, 8);
    gtk_widget_set_margin_top(box, 8);
    gtk_widget_set_margin_bottom(box, 8);
    
    // Add icon
    GtkWidget* icon = gtk_image_new_from_icon_name("application-zip");
    gtk_image_set_pixel_size(GTK_IMAGE(icon), 32);
    gtk_box_append(GTK_BOX(box), icon);
    
    // Create info box
    GtkWidget* infoBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    
    // Add title label
    GtkWidget* titleLabel = gtk_label_new(comic->getDisplayName().c_str());
    gtk_label_set_xalign(GTK_LABEL(titleLabel), 0.0);
    gtk_label_set_ellipsize(GTK_LABEL(titleLabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_max_width_chars(GTK_LABEL(titleLabel), 40);
    gtk_widget_add_css_class(titleLabel, "title-4");
    gtk_box_append(GTK_BOX(infoBox), titleLabel);
    
    // Add size label
    uint64_t fileSize = FileUtils::fileExists(comic->fullpath) ? 
        static_cast<uint64_t>(g_file_test(comic->fullpath.c_str(), G_FILE_TEST_EXISTS) ? 
        g_stat(comic->fullpath.c_str(), nullptr).st_size : 0) : 0;
    std::string sizeStr = FileUtils::getFileSizeString(fileSize);
    GtkWidget* sizeLabel = gtk_label_new(sizeStr.c_str());
    gtk_label_set_xalign(GTK_LABEL(sizeLabel), 0.0);
    gtk_widget_add_css_class(sizeLabel, "dim-label");
    gtk_box_append(GTK_BOX(infoBox), sizeLabel);
    
    gtk_box_append(GTK_BOX(box), infoBox);
    
    // Add cover placeholder
    GtkWidget* coverBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(coverBox, 64, -1);
    gtk_box_append(GTK_BOX(box), coverBox);
    
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), box);
    gtk_list_box_append(GTK_LIST_BOX(listBox), row);
}

void ComicList::showEmptyMessage(const std::string& message) {
    clearList();
    gtk_label_set_text(GTK_LABEL(emptyLabel), message.c_str());
    gtk_list_box_append(GTK_LIST_BOX(listBox), emptyLabel);
}

} // namespace Librairie
