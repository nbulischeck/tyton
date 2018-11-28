CC = gcc
INCL := `pkg-config --cflags --libs gtk+-3.0`
INCL += `pkg-config --cflags --libs libnotify`

systemd := $(shell pkg-config --cflags --libs libsystemd)
journal := $(shell pkg-config --cflags --libs libsystemd-journal)

ifneq ($(systemd),)
 INCL += `pkg-config --cflags --libs libsystemd`
else
 ifneq ($(journal),)
  INCL += `pkg-config --cflags --libs libsystemd-journal`
 endif
endif

daemon:
	$(CC) -o tyton-notify tyton-notify.c $(INCL)

install:
	@ sudo cp tyton-notify /usr/bin/tyton-notify
	@ sudo cp tyton-notify.desktop /etc/xdg/autostart/tyton-notify.desktop

clean:
	rm -f tyton-notify

uninstall:
	@ sudo rm -f /usr/bin/tyton-notify /etc/xdg/autostart/tyton-notify.desktop
