#include <string.h>
#include <stdio.h>
#include <math.h>

#define GLIB_DISABLE_DEPRECATION_WARNINGS

#include <gst/gst.h>

GstElement *pipeline;
GError *error = NULL;
GstBus *bus;
guint watch_id;
GMainLoop *loop;


#define NUMEL (1000)
FILE *gnuplot;
int arr[NUMEL];


static gboolean
message_handler (GstBus * bus, GstMessage * message, gpointer timer)
{

    if (message->type == GST_MESSAGE_ELEMENT) {
        const GstStructure *s = gst_message_get_structure (message);
        const gchar *name = gst_structure_get_name (s);

        if (strcmp (name, "level") == 0) {
            gint channels;
            GstClockTime endtime;
            gdouble rms_dB, peak_dB, decay_dB;
            gdouble rms;
            const GValue *array_val;
            const GValue *value;
            GValueArray *rms_arr, *peak_arr, *decay_arr;
            gint i;

            if (!gst_structure_get_clock_time (s, "endtime", &endtime))
                g_warning ("Could not parse endtime");

            /* the values are packed into GValueArrays with the value per channel */
            array_val = gst_structure_get_value (s, "rms");
            rms_arr = (GValueArray *) g_value_get_boxed (array_val);

            array_val = gst_structure_get_value (s, "peak");
            peak_arr = (GValueArray *) g_value_get_boxed (array_val);

            array_val = gst_structure_get_value (s, "decay");
            decay_arr = (GValueArray *) g_value_get_boxed (array_val);

            /* we can get the number of channels as the length of any of the value
             * arrays */
            channels = rms_arr->n_values;
            g_print ("endtime: %" GST_TIME_FORMAT ", channels: %d\n",
                    GST_TIME_ARGS (endtime), channels);
            for (i = 0; i < channels; ++i) {

                g_print ("channel %d\n", i);
                value = g_value_array_get_nth (rms_arr, i);
                rms_dB = g_value_get_double (value);

                value = g_value_array_get_nth (peak_arr, i);
                peak_dB = g_value_get_double (value);

                value = g_value_array_get_nth (decay_arr, i);
                decay_dB = g_value_get_double (value);
                g_print ("    RMS: %f dB, peak: %f dB, decay: %f dB\n",
                        rms_dB, peak_dB, decay_dB);

                /* converting from dB to normal gives us a value between 0.0 and 1.0 */
                rms = pow (10, rms_dB / 20);
                g_print ("    normalized rms value: %f\n", rms);
              
            
                //arr[time_int] = rms;

                //fprintf(gnuplot, "set style line 1\n");
                fprintf(gnuplot, "plot [-1:1001] [0:101] '-' with linespoints linestyle 1\n");
                for (int i = 0; i < NUMEL; i++) {
                    // plot
                    fprintf(gnuplot, "%d\n", arr[i]);

                    // shift value for next iteration OR get new value
                    if (i+1 < NUMEL) {
                        arr[i] = arr[i+1];
                    } else {
                        arr[i] = rms * 100;
                    } 
                
                }
                fprintf(gnuplot, "e\n");
                fflush(gnuplot);
            }
        }
    }
    /* we handled the message we want, and ignored the ones we didn't want.
     * so the core can unref the message for us */
    return TRUE;
}

int
main (int argc, char** argv)
{
    gst_init (&argc, &argv);


    #define MICROPHONE "hw:1,0,0" 

    char* pipe_desc = \
        " alsasrc device="MICROPHONE"                               \n"
        "   ! level post-messages=TRUE name=level interval=10000000   \n"
        "   ! fakesink                                              \n";

    pipeline = gst_parse_launch (pipe_desc, &error);
    if (error) {
        g_printerr ("pipeline parsing error: %s\n", error->message);
        g_error_free (error);
        return 1;
    }

    printf("%s", pipe_desc);


    gnuplot = popen("gnuplot", "w");
    for (int i = 0; i < NUMEL; i++) {
        arr[i] = 0;
    }


    GTimer* timer = g_timer_new ();
    bus = gst_element_get_bus (pipeline);
    watch_id = gst_bus_add_watch (bus, message_handler, timer);

    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    /* we need to run a GLib main loop to get the messages */
    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);

    g_source_remove (watch_id);
    g_main_loop_unref (loop);
    return 0;
}

