/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionGUI.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 07:30:00 by adnen             #+#    #+#             */
/*   Updated: 2026/03/22 07:33:00 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScorpionGUI.hpp"

// ============================================================================
// Orthodox Canonical Form
// ============================================================================

ScorpionGUI::ScorpionGUI(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    _currentFile = "";
    _window = NULL;
    _treeView = NULL;
    _listStore = NULL;
    _fileLabel = NULL;
    _statusLabel = NULL;
    _setupCSS();
    _setupWindow();
}

ScorpionGUI::~ScorpionGUI()
{
}

ScorpionGUI::ScorpionGUI(const ScorpionGUI &src)
{
    (void)src;
}

ScorpionGUI &ScorpionGUI::operator=(const ScorpionGUI &other)
{
    (void)other;
    return *this;
}

void ScorpionGUI::run()
{
    gtk_widget_show_all(_window);
    gtk_main();
}

// ============================================================================
// Dark Theme CSS
// ============================================================================

void ScorpionGUI::_setupCSS()
{
    GtkCssProvider *css = gtk_css_provider_new();

    const char *style =
        "window {"
        "	background-color: #0d1117;"
        "}"
        ""
        "label {"
        "	color: #c9d1d9;"
        "	font-size: 14px;"
        "}"
        ""
        ".title-label {"
        "	font-size: 26px;"
        "	font-weight: bold;"
        "	color: #58a6ff;"
        "}"
        ""
        ".file-label {"
        "	font-size: 13px;"
        "	color: #8b949e;"
        "	font-style: italic;"
        "}"
        ""
        ".status-label {"
        "	font-size: 12px;"
        "	color: #3fb950;"
        "}"
        ""
        "button {"
        "	background-image: none;"
        "	background-color: #21262d;"
        "	color: #c9d1d9;"
        "	border: 1px solid #30363d;"
        "	border-radius: 6px;"
        "	padding: 8px 16px;"
        "	font-size: 13px;"
        "	font-weight: 600;"
        "}"
        ""
        "button:hover {"
        "	background-color: #30363d;"
        "	border-color: #58a6ff;"
        "}"
        ""
        ".btn-open {"
        "	background-color: #238636;"
        "	border-color: #2ea043;"
        "	color: white;"
        "}"
        ""
        ".btn-open:hover {"
        "	background-color: #2ea043;"
        "}"
        ""
        ".btn-delete {"
        "	background-color: #da3633;"
        "	border-color: #f85149;"
        "	color: white;"
        "}"
        ""
        ".btn-delete:hover {"
        "	background-color: #f85149;"
        "}"
        ""
        "treeview {"
        "	background-color: #161b22;"
        "	color: #c9d1d9;"
        "	font-size: 13px;"
        "}"
        ""
        "treeview:selected {"
        "	background-color: #1f6feb;"
        "}"
        ""
        "treeview header button {"
        "	background-image: none;"
        "	background-color: #21262d;"
        "	color: #8b949e;"
        "	border-bottom: 2px solid #30363d;"
        "	border-radius: 0;"
        "	font-weight: bold;"
        "	font-size: 12px;"
        "	text-transform: uppercase;"
        "	letter-spacing: 1px;"
        "}";

    gtk_css_provider_load_from_data(css, style, -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(), GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(css);
}

// ============================================================================
// Window Setup
// ============================================================================

void ScorpionGUI::_setupWindow()
{
    // --- Window ---
    _window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(_window), "Scorpion — Metadata Viewer");
    gtk_window_set_default_size(GTK_WINDOW(_window), 750, 550);
    g_signal_connect(_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // --- Main vertical box ---
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 24);
    gtk_container_add(GTK_CONTAINER(_window), vbox);

    // --- Title ---
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title),
                         "<span size='xx-large' weight='bold' color='#58a6ff'>"
                         "🦂  Scorpion Metadata Viewer</span>");
    gtk_style_context_add_class(gtk_widget_get_style_context(title),
                                "title-label");
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);

    // --- File label ---
    _fileLabel = gtk_label_new("No file selected");
    gtk_style_context_add_class(gtk_widget_get_style_context(_fileLabel),
                                "file-label");
    gtk_box_pack_start(GTK_BOX(vbox), _fileLabel, FALSE, FALSE, 0);

    // --- Button row ---
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(hbox, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);

    GtkWidget *btnOpen = gtk_button_new_with_label("📂  Open Image");
    gtk_style_context_add_class(gtk_widget_get_style_context(btnOpen),
                                "btn-open");
    g_signal_connect(btnOpen, "clicked", G_CALLBACK(_onOpenClicked), this);
    gtk_box_pack_start(GTK_BOX(hbox), btnOpen, FALSE, FALSE, 0);

    GtkWidget *btnDelete = gtk_button_new_with_label("🗑️  Delete EXIF");
    gtk_style_context_add_class(gtk_widget_get_style_context(btnDelete),
                                "btn-delete");
    g_signal_connect(btnDelete, "clicked", G_CALLBACK(_onDeleteClicked), this);
    gtk_box_pack_start(GTK_BOX(hbox), btnDelete, FALSE, FALSE, 0);

    // --- Status label ---
    _statusLabel = gtk_label_new("");
    gtk_style_context_add_class(gtk_widget_get_style_context(_statusLabel),
                                "status-label");
    gtk_box_pack_start(GTK_BOX(vbox), _statusLabel, FALSE, FALSE, 0);

    // --- TreeView ---
    _listStore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    _treeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(_listStore));
    g_object_unref(_listStore);

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();

    GtkTreeViewColumn *col1 = gtk_tree_view_column_new_with_attributes(
        "Property", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_min_width(col1, 220);
    gtk_tree_view_column_set_resizable(col1, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(_treeView), col1);

    GtkTreeViewColumn *col2 = gtk_tree_view_column_new_with_attributes(
        "Value", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_expand(col2, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(_treeView), col2);

    gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(_treeView),
                                 GTK_TREE_VIEW_GRID_LINES_HORIZONTAL);

    // --- Scrollable area ---
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scroll), _treeView);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);
}

// ============================================================================
// Metadata Display
// ============================================================================

void ScorpionGUI::_addMetadataRow(const std::string &property,
                                  const std::string &value)
{
    GtkTreeIter iter;
    gtk_list_store_append(_listStore, &iter);
    gtk_list_store_set(_listStore, &iter, 0, property.c_str(), 1, value.c_str(),
                       -1);
}

void ScorpionGUI::_clearList()
{
    gtk_list_store_clear(_listStore);
}

void ScorpionGUI::_analyzeFile(const std::string &filename)
{
    _clearList();
    _currentFile = filename;
    gtk_label_set_text(GTK_LABEL(_fileLabel), filename.c_str());

    // Capture stdout from the existing Scorpion analysis
    std::stringstream buffer;
    std::streambuf *oldCout = std::cout.rdbuf(buffer.rdbuf());
    std::streambuf *oldCerr = std::cerr.rdbuf(buffer.rdbuf());

    Scorpion::FileIdentification(filename);

    std::cout.rdbuf(oldCout);
    std::cerr.rdbuf(oldCerr);

    // Parse each line and populate the TreeView
    std::string line;
    int count = 0;
    while (std::getline(buffer, line))
    {
        // Strip ANSI escape codes (BOLD_CYAN, GREEN, etc.)
        std::string clean;
        bool inEscape = false;
        for (size_t i = 0; i < line.size(); i++)
        {
            if (line[i] == '\033')
                inEscape = true;
            else if (inEscape && line[i] == 'm')
                inEscape = false;
            else if (!inEscape)
                clean += line[i];
        }

        // Look for "key : value" pattern (used by _readTagValue and
        // _fileAnalysis)
        size_t sep = clean.find(" : ");
        if (sep == std::string::npos) continue;

        std::string key = clean.substr(0, sep);
        std::string val = clean.substr(sep + 3);

        // Remove leading markers like [EXIF] [FILE] [GPS]
        size_t bracket = key.rfind(']');
        if (bracket != std::string::npos) key = key.substr(bracket + 1);

        // Trim whitespace
        while (!key.empty() && key[0] == ' ') key.erase(0, 1);
        while (!key.empty() && key.back() == ' ') key.pop_back();
        while (!val.empty() && val[0] == ' ') val.erase(0, 1);
        while (!val.empty() && val.back() == ' ') val.pop_back();

        if (!key.empty())
        {
            _addMetadataRow(key, val);
            count++;
        }
    }

    // Update status
    std::string status = "Found " + std::to_string(count) + " metadata entries";
    if (count == 0) status = "No metadata found in this file";
    gtk_label_set_text(GTK_LABEL(_statusLabel), status.c_str());
}

// ============================================================================
// GTK Callbacks
// ============================================================================

void ScorpionGUI::_onOpenClicked(GtkWidget *widget, gpointer data)
{
    (void)widget;
    ScorpionGUI *self = (ScorpionGUI *)data;

    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Open Image", GTK_WINDOW(self->_window), GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    // File filter for images
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Images (jpg, png, gif, bmp, webp)");
    gtk_file_filter_add_pattern(filter, "*.jpg");
    gtk_file_filter_add_pattern(filter, "*.jpeg");
    gtk_file_filter_add_pattern(filter, "*.png");
    gtk_file_filter_add_pattern(filter, "*.gif");
    gtk_file_filter_add_pattern(filter, "*.bmp");
    gtk_file_filter_add_pattern(filter, "*.webp");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    GtkFileFilter *allFilter = gtk_file_filter_new();
    gtk_file_filter_set_name(allFilter, "All files");
    gtk_file_filter_add_pattern(allFilter, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), allFilter);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename =
            gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        self->_analyzeFile(filename);
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

void ScorpionGUI::_onDeleteClicked(GtkWidget *widget, gpointer data)
{
    (void)widget;
    ScorpionGUI *self = (ScorpionGUI *)data;

    if (self->_currentFile.empty())
    {
        gtk_label_set_text(GTK_LABEL(self->_statusLabel), "No file selected");
        return;
    }

    // Confirmation dialog
    GtkWidget *confirm = gtk_message_dialog_new(
        GTK_WINDOW(self->_window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
        GTK_BUTTONS_YES_NO, "Delete all EXIF metadata from this file?");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(confirm), "%s",
                                             self->_currentFile.c_str());

    int response = gtk_dialog_run(GTK_DIALOG(confirm));
    gtk_widget_destroy(confirm);

    if (response != GTK_RESPONSE_YES) return;

    // Call the MetadataEditor
    std::stringstream buf;
    std::streambuf *old = std::cout.rdbuf(buf.rdbuf());

    bool success = MetadataEditor::deleteMetadata(self->_currentFile);

    std::cout.rdbuf(old);

    if (success)
    {
        gtk_label_set_text(GTK_LABEL(self->_statusLabel),
                           "✅ EXIF metadata deleted!");
        self->_analyzeFile(self->_currentFile);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(self->_statusLabel),
                           "❌ Failed to delete metadata");
    }
}
