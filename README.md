SpaceAPI-GTK
============

A panel Applet using GTK2 for showing the Status of a Hackerspace. Compatible with the SpaceAPI version 0.13.

Installation:
-------------

Requirements:

- Standard tools like gcc, pkg-config etc.
- curl
- gtk+-2.0
- json-c
- libnotify - Only needed when compiling with libnotify support.

Use the following command for getting the sources:

    git clone https://github.com/hanez/SpaceAPI-GTK.git

Change into the new directory:

    cd SpaceAPI-GTK

To compile the sources just run:

    make

This will compile dooris with libnotify support.

If you don't want notifications run the following comand:

    gcc `pkg-config --cflags --libs gtk+-2.0 --libs libcurl --libs json-c` -o spaceapi-gtk spaceapi-gtk.c

Installation
------------

Just copy "dooris" to a folder in your $PATH. I do:

    cp ./spaceapi-gtk ~/bin/

You can copy the file to /usr/bin to make it available for all users but I don't. If you want that just run:

    sudo cp ./spaceapi-gtk /usr/bin/

Configuration:
--------------

Configuration needs to be done before compiling. There are some parameters you can set in the head of the spaceapi-gtk.c file where variables are declared. A configuration file will at this point make no sense.

Usage:
------

Run spaceapi-gtk with:

    ./spaceapi-gtk & 

Or if in your $PATH:

    spaceapi-gtk

Status Icon "Pesthörnchen" colors:

- Red = Door closed
- Yellow = Door open

