CC = gcc
INCL := `pkg-config --cflags --libs gtk+-3.0`
INCL += `pkg-config --cflags --libs libnotify`
INCL += -lsystemd

all:
	$(CC) -o tyton-notify tyton-notify.c $(INCL)

install:
	@ sudo cp tyton-notify /usr/bin/tyton-notify
	@ sudo cp tyton-notify.desktop /etc/xdg/autostart/tyton-notify.desktop

clean:
	rm -f tyton-notify

uninstall:
	@ sudo rm -f /usr/bin/tyton-notify /etc/xdg/autostart/tyton-notify.desktop
