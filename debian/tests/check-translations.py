#!/usr/bin/python3
"""Verify shipped catalogs, rather than just translated PO source files."""
import gettext
import io
import pathlib
import subprocess
import sys

languages = ("de", "es", "fr", "id", "it", "pt", "pt_BR", "ru")
required = "_Display Settings..."
root = pathlib.Path(sys.argv[1])

for language in languages:
    source = pathlib.Path("po") / (language + ".po")
    expected = gettext.GNUTranslations(io.BytesIO(subprocess.check_output(
        ["msgfmt", "--check", "--output-file=-", str(source)])))
    translated = expected.gettext(required)
    assert translated and translated != required, (language, "Missing source translation", required)
    with (root / language / "LC_MESSAGES/xfdesktop.mo").open("rb") as catalog:
        actual = gettext.GNUTranslations(catalog)
    assert actual.gettext(required) == translated, (
        language, "Missing/stale menu translation", actual.gettext(required), translated)
    # Compare the complete non-header catalog to catch other stale translations
    # from the upstream GMO files, including plural entries.
    wanted = {key: value for key, value in expected._catalog.items() if key != ""}
    shipped = {key: value for key, value in actual._catalog.items() if key != ""}
    assert shipped == wanted, (language, "Shipped catalog differs from current PO")
    print("PASS shipped translations", language, len(wanted), repr(translated))
