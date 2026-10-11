#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <string.h>
#include <libxfce4ui/libxfce4ui.h>
#include "menu.h"

static gchar *directory;
static gchar *executable;
static guint launches;
static guint errors;

gboolean __wrap_xfce_spawn_command_line(GdkScreen *screen, const gchar *command,
                                       gboolean terminal, gboolean startup,
                                       gboolean child, GError **error);
gint __wrap_xfce_message_dialog(GtkWindow *parent, const gchar *title,
                               const gchar *icon, const gchar *primary,
                               const gchar *secondary, const gchar *button, ...);

/* Intercept only the external launch and modal dialog. Exercise the real menu
 * construction, activation callback, PATH discovery and command quoting. */
gboolean
__wrap_xfce_spawn_command_line(GdkScreen *screen, const gchar *command,
                               gboolean terminal, gboolean startup,
                               gboolean child, GError **error) {
    gchar **argv = NULL;
    gint argc;
    g_assert_true(g_shell_parse_argv(command, &argc, &argv, NULL));
    g_assert_cmpint(argc, ==, 1);
    g_assert_false(terminal);
    g_assert_true(startup);
    g_assert_true(child);
    g_assert_nonnull(screen);
    gboolean available = g_file_test(argv[0], G_FILE_TEST_IS_EXECUTABLE);
    if (available) {
        g_assert_cmpstr(argv[0], ==, executable);
        launches++;
    } else {
        g_set_error_literal(error, G_SPAWN_ERROR, G_SPAWN_ERROR_NOENT, "Executable disappeared");
    }
    g_strfreev(argv);
    return available;
}

gint
__wrap_xfce_message_dialog(GtkWindow *parent, const gchar *title,
                           const gchar *icon, const gchar *primary,
                           const gchar *secondary, const gchar *button, ...) {
    g_assert_true(GTK_IS_WINDOW(parent));
    g_assert_cmpstr(title, ==, "Launch Error");
    g_assert_cmpstr(icon, ==, "dialog-error");
    g_assert_nonnull(strstr(primary, "xfce4-display-settings"));
    g_assert_cmpstr(secondary, ==, "Executable disappeared");
    errors++;
    return GTK_RESPONSE_ACCEPT;
}

static void
test_display_settings(void) {
    GtkWidget *parent = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *menu = gtk_menu_new();
    g_object_ref_sink(menu);
    menu_append_display_settings(GTK_MENU(menu), parent);
    g_assert_null(gtk_container_get_children(GTK_CONTAINER(menu)));

    g_assert_true(g_file_set_contents(executable, "#!/bin/sh\nexit 0\n", -1, NULL));
    g_assert_cmpint(g_chmod(executable, 0644), ==, 0);
    menu_append_display_settings(GTK_MENU(menu), parent);
    g_assert_null(gtk_container_get_children(GTK_CONTAINER(menu)));

    g_assert_cmpint(g_chmod(executable, 0755), ==, 0);
    menu_append_display_settings(GTK_MENU(menu), parent);
    GList *children = gtk_container_get_children(GTK_CONTAINER(menu));
    g_assert_cmpuint(g_list_length(children), ==, 1);
    GtkWidget *item = children->data;
    g_list_free(children);
    gtk_menu_item_activate(GTK_MENU_ITEM(item));
    g_assert_cmpuint(launches, ==, 1);
    g_assert_cmpuint(errors, ==, 0);

    g_assert_cmpint(g_remove(executable), ==, 0);
    gtk_menu_item_activate(GTK_MENU_ITEM(item));
    g_assert_cmpuint(launches, ==, 1);
    g_assert_cmpuint(errors, ==, 1);

    GtkWidget *absent_menu = gtk_menu_new();
    g_object_ref_sink(absent_menu);
    menu_append_display_settings(GTK_MENU(absent_menu), parent);
    g_assert_null(gtk_container_get_children(GTK_CONTAINER(absent_menu)));
    gtk_widget_destroy(absent_menu);
    g_object_unref(absent_menu);
    gtk_widget_destroy(menu);
    g_object_unref(menu);
    gtk_widget_destroy(parent);
}

int
main(int argc, char **argv) {
    gtk_test_init(&argc, &argv, NULL);
    directory = g_dir_make_tmp("xfdesktop-menu-XXXXXX", NULL);
    g_assert_nonnull(directory);
    gchar *quoted_directory = g_build_filename(directory, "space and 'quote", NULL);
    g_assert_cmpint(g_mkdir(quoted_directory, 0700), ==, 0);
    executable = g_build_filename(quoted_directory, "xfce4-display-settings", NULL);
    g_setenv("PATH", quoted_directory, TRUE);
    g_test_add_func("/menu/display-settings/discovery-launch-loss", test_display_settings);
    gint result = g_test_run();
    g_rmdir(quoted_directory);
    g_rmdir(directory);
    g_free(executable);
    g_free(quoted_directory);
    g_free(directory);
    return result;
}
