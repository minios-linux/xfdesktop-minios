if ENABLE_TESTS

noinst_PROGRAMS = test-display-settings-menu
noinst_PROGRAMS += test-display-settings-translation
test_display_settings_menu_SOURCES = test-display-settings-menu.c ../src/menu.c
test_display_settings_menu_CFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/common \
	$(GTK_CFLAGS) $(LIBXFCE4UI_CFLAGS) $(LIBXFCE4UTIL_CFLAGS) $(XFCONF_CFLAGS) \
	$(GARCON_CFLAGS) $(GARCON_GTK3_CFLAGS)
test_display_settings_menu_LDADD = $(top_builddir)/common/libxfdesktop.la \
	$(GTK_LIBS) $(LIBXFCE4UI_LIBS) $(LIBXFCE4UTIL_LIBS) $(XFCONF_LIBS) \
	$(LIBXFCE4WINDOWING_LIBS) $(GARCON_LIBS) $(GARCON_GTK3_LIBS)
test_display_settings_menu_LDFLAGS = -Wl,--wrap=xfce_spawn_command_line -Wl,--wrap=xfce_message_dialog
test_display_settings_translation_SOURCES = test-display-settings-translation.c ../src/menu.c
test_display_settings_translation_CFLAGS = $(test_display_settings_menu_CFLAGS)
test_display_settings_translation_LDADD = $(test_display_settings_menu_LDADD)

if ENABLE_FILE_ICONS

noinst_PROGRAMS += \
	test-icon-position-parsing \
	test-icon-position-saving

tests_cflags = \
	-I$(top_srcdir)/src \
	$(GTK_CFLAGS) \
	$(LIBXFCE4UTIL_CFLAGS) \
	$(LIBXFCE4WINDOWING_CFLAGS) \
	$(LIBYAML_CFLAGS)

tests_ldadd = \
	$(GTK_LIBS) \
	$(LIBXFCE4UTIL_LIBS) \
	$(LIBXFCE4WINDOWING_LIBS) \
	$(LIBYAML_LIBS)

test_icon_position_parsing_SOURCES = \
	test-icon-position-parsing.c
test_icon_position_parsing_CFLAGS = $(tests_cflags)
test_icon_position_parsing_LDADD = $(tests_ldadd)

test_icon_position_saving_SOURCES = \
	test-icon-position-saving.c
test_icon_position_saving_CFLAGS = $(tests_cflags)
test_icon_position_saving_LDADD = $(tests_ldadd)

endif

endif

EXTRA_DIST = \
	test-display-settings-menu.c \
	test-display-settings-translation.c \
	meson.build \
	test-icon-position-parsing.c \
	test-icon-position-saving.c
