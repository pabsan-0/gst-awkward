## Demos

Some samples that were used to figure out how to do the things.

- `gstlevel.c`: A demo pipeline of the gstreamer `level` plugin, taken from its documentation. 
- `gnuplot.c`: A very basic `gnuplot` demo from stdin.
- `gstlevel_plots_stdin.c`: Gstreamer audio pipe with its rms value being plotted.
- `gstlevel_plot_buffer.c`: Gstreamer audio pipe with its rms, peak and decay values being plotted. Can be used to further understand the parameters of the `level` plugin (see `gst-inspect-1.0 level`).
