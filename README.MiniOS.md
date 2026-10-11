# Xfdesktop MiniOS fork

This fork retains Xfdesktop 4.20.1 and the Debian Trixie packaging for
`xfdesktop4` and `xfdesktop4-data`. The MiniOS package revision is
`4.20.1-1minios2`; the upstream program version remains `4.20.1`.

Upstream: https://github.com/xfce-mirror/xfdesktop

Base tag: `xfdesktop-4.20.1`, commit
`5507be26484146bdccb8652677e9c8d76f6e0c84`.

## Desktop context menu

The desktop background context menu includes **Display Settings…** in all icon
modes, including no icons. In file/launcher and minimized-window icon modes it
appears directly after **Desktop Settings…**. The optional **Applications**
submenu can follow it; Display Settings is not guaranteed to be the last item.
The action appears whenever `xfce4-display-settings` is an executable
file in the desktop process's `PATH`. Availability is checked whenever the
menu opens and again before launch. If it disappears while the menu is open,
activating the item shows the native launch-error dialog. Installing or removing the executable takes
effect on the next menu opening; no package-manager query or desktop restart
is required.

The action is specific to the desktop background and uses the upstream native
GTK menu, icon theme, Xfce launcher, and launch-error dialog. The shared
`minios_gui` helpers are Python APIs; this C desktop-shell integration reuses
the existing Xfdesktop facilities. It adds no stylesheet or runtime dependency
on a particular settings package or package manager.

Translations use the existing `xfdesktop` gettext domain and are maintained
with the parent `minios-live` workspace's `lokit.yaml`, `lokit-terms.yaml` and
`lokit.lock`. Run `lokit status --target xfdesktop-minios` from that workspace
root; the PO catalogs remain in this repository.

The package rebuilds all GMO catalogs from the PO inputs and checks the shipped
translations for de, es, fr, id, it, pt, pt_BR and ru. This avoids reusing the
upstream precompiled catalogs, which do not contain the MiniOS menu action.

## Development builds

Build in the persistent Trixie development container as `builder`, using a
container-local source copy and `dpkg-buildpackage -b -us -uc`. The packaging
bootstraps the Git checkout with `autogen.sh`, keeps upstream tests enabled,
and explicitly runs the menu discovery/launch/error regression test and the
icon-position save/parse round trip under Xvfb. The menu test intercepts the
external launch and modal dialog, so it does not modify a running desktop.

For source validation, use the original archive named below as
`xfdesktop4_4.20.1.orig.tar.bz2`, create the quilt source package from an external
staging copy, and rebuild the extracted `.dsc`. Regenerated `po/*.gmo` files
are excluded from the source diff; the PO catalogs remain the translation
inputs. Clean Trixie amd64 and i386 builds validate the architecture-dependent
package, with the common data package built once on amd64.

Xfdesktop 4.20 requires the modern GLib/Xfce stack supplied by Trixie; this fork
does not change the requirements to support the older Bionic desktop stack.
Generated build and package artifacts belong outside this source checkout.

The Debian packaging originates from
https://salsa.debian.org/xfce-team/desktop/xfdesktop4 and source version
`4.20.1-1`. Its upstream archive SHA-256 is
`acccde849265bbf4093925ba847977b7abf70bb2977e4f78216570e887c157b8`.
