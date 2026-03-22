/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionGUI.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 07:30:00 by adnen             #+#    #+#             */
/*   Updated: 2026/03/22 07:32:59 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SCORPION_GUI_HPP__
#define __SCORPION_GUI_HPP__

#include <gtk/gtk.h>

#include "../Spider/includes.hpp"
#include "MetadataEditor.hpp"
#include "Scorpion.hpp"

class ScorpionGUI
{
public:
    ScorpionGUI(int argc, char **argv);
    ~ScorpionGUI();
    ScorpionGUI(const ScorpionGUI &src);
    ScorpionGUI &operator=(const ScorpionGUI &other);

    void run();

private:
    GtkWidget *_window;
    GtkWidget *_treeView;
    GtkListStore *_listStore;
    GtkWidget *_fileLabel;
    GtkWidget *_statusLabel;
    std::string _currentFile;

    void _setupWindow();
    void _setupCSS();
    void _addMetadataRow(const std::string &property, const std::string &value);
    void _analyzeFile(const std::string &filename);
    void _clearList();

    // Callbacks GTK (static car GTK est une API C)
    static void _onOpenClicked(GtkWidget *widget, gpointer data);
    static void _onDeleteClicked(GtkWidget *widget, gpointer data);
};

#endif
