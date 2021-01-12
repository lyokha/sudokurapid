all:

install-data-hook:
	cd ..; $(XDG_DESKTOP_MENU) install --novendor ./sudokuRapidQt.desktop
	cd ..; $(XDG_ICON_RESOURCE) install --novendor --size 48 ./sudokuRapidQt-48x48.png sudokuRapidQt
uninstall-hook:
	$(XDG_ICON_RESOURCE) uninstall --size 48 sudokuRapidQt
	$(XDG_DESKTOP_MENU) uninstall sudokuRapidQt.desktop
