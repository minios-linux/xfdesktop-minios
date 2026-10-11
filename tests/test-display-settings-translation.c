/* SPDX-License-Identifier: GPL-2.0-or-later */
#include <config.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <libxfce4util/libxfce4util.h>
#include <locale.h>
#include "menu.h"

/* Probe the real menu builder against the catalogs installed by this build.
 * LANGUAGE selects the catalog; callers supply a non-C UTF-8 locale. */
int
main(int argc, char **argv) {
    if (argc != 2)
        return 2;
    gchar *catalog_dir = g_strdup(argv[1]);
    setlocale(LC_ALL, "");
    xfce_textdomain(GETTEXT_PACKAGE, catalog_dir, "UTF-8");
    gtk_init(&argc, &argv);
    gchar *directory = g_dir_make_tmp("xfdesktop-translation-XXXXXX", NULL);
    gchar *executable = g_build_filename(directory, "xfce4-display-settings", NULL);
    g_assert_true(g_file_set_contents(executable, "#!/bin/sh\nexit 0\n", -1, NULL));
    g_assert_cmpint(g_chmod(executable, 0755), ==, 0);
    g_setenv("PATH", directory, TRUE);
    GtkWidget *parent = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *menu = gtk_menu_new();
    g_object_ref_sink(menu);
    menu_append_display_settings(GTK_MENU(menu), parent);
    GList *children = gtk_container_get_children(GTK_CONTAINER(menu));
    g_assert_cmpuint(g_list_length(children), ==, 1);
    const gchar *label = gtk_menu_item_get_label(GTK_MENU_ITEM(children->data));
    g_assert_nonnull(label);
    g_assert_cmpstr(label, !=, "_Display Settings...");
    g_assert_true(g_utf8_validate(label, -1, NULL));
    g_print("PASS menu translation %s: %s\n", g_getenv("LANGUAGE"), label);
    g_list_free(children);
    gtk_widget_destroy(menu);
    g_object_unref(menu);
    gtk_widget_destroy(parent);
    g_remove(executable);
    g_rmdir(directory);
    g_free(executable);
    g_free(directory);
    g_free(catalog_dir);
    return 0;
}
