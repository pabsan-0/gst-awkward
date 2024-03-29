# gst-awkward  

A small gstreamer application that plays music, but shuts up whenever someone speaks. Quit awkward silences today!

## Usage

- Match dependencies:
    - Gstreamer
    - Alsa-utils [optional]: `apt install alsa-utils`
    - Gnuplot [optional]: `apt install gnuplot`
- Clone this repo `git clone https://github.com/pabsan-0/gst-awkward`
- Set your device configuration in the `src/main.c` `#define`s 
- Build the tool: `make`
- Run the app with `./main.o`

## The audio essentials

- Audio loopback `sudo modprobe snd-aloop`
- List mic-like devices `arecord -l`
- List speaker-like devices `aplay -l`
- Hardware spec: `hw:x,y,z` `hw:card,device,subdevice`
  - `x`: Card
  - `y`: Device
  - `z`: Subdevice
- Gstreamer elements:
    - `alsasink`, `alsasrc`: Advanced Linux Sound Architecture
    - `audioconvert`
    - `volume`, `level`: for setting/reading levels
    - `decodebin`, `autovideosink`, `autovideosrc`

## Snippets:
```
# Playing audio file in a loop
gst-launch-1.0 multifilesrc loop=1 location=media/short.mp3 ! decodebin ! audioconvert ! alsasink

# Introducing delay
gst-launch-1.0 audiotestsrc ! queue max-size-buffers=0 max-size-time=0 max-size-bytes=0 min-threshold-time=1000000000000 ! autoaudiosink

# Mic to speaker with native delay (fractions of a sec)
gst-launch-1.0 alsasrc device="hw:3,0,0" ! alsasink 

# Mic to speaker fancier
gst-launch-1.0 alsasrc device="hw:3,0,0" ! volume volume=1.5 ! level message=TRUE ! alsasink 
```


## Links 

- Linux audio 101 small tutorial [TheSalarKhan/Linux-Audio-Loopback-Device](https://github.com/TheSalarKhan/Linux-Audio-Loopback-Device)
- On introducing delay on a pipeline https://stackoverflow.com/a/17218113
- Level plugin example https://github.com/krad-radio/gstreamer-plugins-good-krad/blob/master/tests/examples/level/level-example.c


## Acknowledgements

- Song bit from The Engagement by Silent Partner: https://www.youtube.com/watch?v=_TcAQEufZcU



<!-- 

cmake, Makefile
.editorconf
settings.ini glib

Runtime dependencies of programs:

$ ldd ./main.o
        libgstreamer-1.0.so.0 => /lib/x86_64-linux-gnu/libgstreamer-1.0.so.0 (0x00007f375de2b000)
        ...
$ apt-file search libgstreamer-1.0.so.0
libgstreamer1.0-0: /usr/lib/x86_64-linux-gnu/libgstreamer-1.0.so.0
libgstreamer1.0-0: /usr/lib/x86_64-linux-gnu/libgstreamer-1.0.so.0.2001.0
libgstreamer1.0-0: /usr/lib/x86_64-linux-gnu/libgstreamer-1.0.so.0.2003.0
libgstreamer1.0-dev: /usr/share/gdb/auto-load/usr/lib/x86_64-linux-gnu/libgstreamer-1.0.so.0.2001.0-gdb.py
libgstreamer1.0-dev: /usr/share/gdb/auto-load/usr/lib/x86_64-linux-gnu/libgstreamer-1.0.so.0.2003.0-gdb.py

```
# GST_DELAY 10^6
GST_DELAY (){
    echo "queue max-size-buffers=0 max-size-time=0 max-size-bytes=0 min-threshold-time=`echo $1 | bc`"
}

```
-->

