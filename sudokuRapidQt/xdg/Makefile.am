all:

install-data-hook:
	$(XDG_DESKTOP_MENU) install --novendor ./sudokuRapidQt.desktop
	$(XDG_ICON_RESOURCE) install --novendor --size 48 ./sudokuRapidQt-48x48.png sudokuRapidQt
uninstall-hook:
	$(XDG_ICON_RESOURCE) uninstall --size 48 sudokuRapidQt
	$(XDG_DESKTOP_MENU) uninstall sudokuRapidQt.desktop
