#include <stdio.h>
#include <unistd.h>


int
main (int argc, char** argv)
{

    FILE *gnuplot = popen("gnuplot", "w");

    // init array of zeros
    int arr[100];
    for (int i = 0; i < 100; i++) {
        arr[i] = 0;
    }

    // Set a few values
    arr[0] = 1;
    arr[2] = 2;
    arr[50] = 2;

    // actual plotting
    fprintf(gnuplot, "plot  [-1:105] [-1:3] '-'\n");
    for (int i = 0; i < 100; i++) {
        fprintf(gnuplot, "%d %d\n", i, arr[i]);
    }
    fprintf(gnuplot, "e\n");
    fflush(gnuplot);

    // user output before quitting
    getchar();
}
