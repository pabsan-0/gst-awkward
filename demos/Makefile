targets:= $(subst .c, ,$(wildcard *.c))

all: $(targets)

$(targets): 	
	gcc $@.c -o $@.o --std=c99 -Wall -g3 `pkg-config --cflags --libs gstreamer-1.0 glib-2.0` -lm

clean:
	rm $(subst .c,.o,$(wildcard *.c))