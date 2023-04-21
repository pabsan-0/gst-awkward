main:
	gcc src/main.c -o main.o --std=c99 -Wall -g3 `pkg-config --cflags --libs gstreamer-1.0 glib-2.0` -lm