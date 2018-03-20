
all: spaceapi-gtk

spaceapi-gtk: spaceapi-gtk.c
	$(CC) -Wall -g `pkg-config --cflags --libs gtk+-2.0 --libs libcurl --libs json-c --libs libnotify` -DLIBNOTIFY -o spaceapi-gtk spaceapi-gtk.c

nonotify: spaceapi-gtk.c
	$(CC) -Wall -g `pkg-config --cflags --libs gtk+-2.0 --libs libcurl --libs json-c` -o spaceapi-gtk spaceapi-gtk.c

clean:
	rm -f spaceapi-gtk
