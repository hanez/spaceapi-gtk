dooris-gtk
==========

Menubar item for GTK2 showing the Dooris status compatible with the SpaceAPI version 0.13.

Installation:
-------------

Requirements:

- Standard tools like gcc, pkg-config etc.
- curl
- gtk+-2.0
- json-c
- libnotify - Only needed when compiling with libnotify support.

Use the following command for getting the sources:

    git clone https://github.com/ccchh/dooris-gtk.git

Change into the new directory:

    cd dooris-gtk

To compile the sources just run:

    make

This will compile dooris with libnotify support.

If you don't want notifications run the following comand:

    gcc `pkg-config --cflags --libs gtk+-2.0 --libs libcurl --libs json-c` -o dooris dooris.c

Installation
------------

Just copy "dooris" to a folder in your $PATH. I do:

    cp ./dooris ~/bin/

You can copy the file to /usr/bin to make it available for all users but I don't. If you want that just run:

    sudo cp ./dooris /usr/bin/

Configuration:
--------------

Configuration needs to be done before compiling. There are some parameters you can set in the head of the dooris.c file where variables are declared. A configuration file will at this point make no sense.

Usage:
------

Run dooris with:

    ./dooris & 

Status Icon "Pesthörnchen" colors:

- Red = Door closed
- Yellow = Door open

