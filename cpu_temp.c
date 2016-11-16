// Compile command -  cc RPiTemperature.c -lrt -o RPiTemperature
// Run     command -  ./RPiTemperature passes ppp, seconds sss
//                    for ppp samples at sss second intervals
// Default            ./RPiTemperature
//                    10 samples at 1 second intervals 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE     *outfile;
char    timeday[30];
double  theseSecs = 0.0;
double  startSecs = 0.0;
double  secs;
struct  timespec tp1;
int     passes  = 10;
int     seconds = 1;


void getTemperature();
void start_time();
void end_time();
void getSecs();
void local_time();


void main(int argc, char *argv[])
{
    int i;
    int param1;

    for (i=1; i<5; i=i+2)
    {
       if (argc > i)
       {
          switch (toupper(argv[i][0]))
          {
               case 'P':
                param1 = 0;
                if (argc > i+1)
                {
                   sscanf(argv[i+1],"%d", &param1);
                   if (param1 > 0) passes = param1;
                }
                break;

                case 'S':
                param1 = 0;
                if (argc > i+1)
                {
                   sscanf(argv[i+1],"%d", &param1);
                   if (param1 > 0) seconds = param1;
                }
                break;
          }
       }
    }
    outfile = fopen("RPiTemperatures.txt", "a+");
    if (outfile == NULL)
    {
        printf ("Cannot open results file \n\n");
        exit (0);
    }

    local_time();
    fprintf(outfile, " Temperature Measurement - Start at %s\n", timeday);
    fprintf(stdout,  " Temperature Measurement - Start at %s\n", timeday);
    fprintf(outfile, "          Using %d samples at %d second intervals\n\n", passes, seconds);
    fprintf(stdout,  "          Using %d samples at %d second intervals\n\n", passes, seconds);
    fprintf(outfile, " Seconds\n");
    fprintf(stdout,  " Seconds\n");
               
    secs = 0.0;
    getTemperature();

    start_time();

    for (i=0; i<passes; i++)
    {
        sleep(seconds);
        end_time();
        getTemperature();
    }
    local_time();
    fprintf(outfile, "\n Temperature Measurement - End at   %s\n", timeday);
    fprintf(stdout,  "\n Temperature Measurement - End at   %s\n", timeday);
    fprintf(stdout,  " Results also in RPiTemperatures.txt where temperatures will\n"
                     " be slightly different as separate function calls are used.\n\n");

    fclose(outfile);
    return;
}

void getTemperature()
{
    fprintf(outfile, " %6.1f  ", secs);
    fprintf(stdout,  " %6.1f  ", secs);
    fflush(outfile);
    fflush (stdout);
    int g = system("/opt/vc/bin/vcgencmd measure_temp >> RPiTemperatures.txt");
    int h = system("/opt/vc/bin/vcgencmd measure_temp");
    return;
}

void local_time()
{
    time_t t;

    t = time(NULL);
    sprintf(timeday, "%s", asctime(localtime(&t)));
    return;
}

void getSecs()
{
     clock_gettime(CLOCK_REALTIME, &tp1);

     theseSecs =  tp1.tv_sec + tp1.tv_nsec / 1e9;           
     return;
}

void start_time()
{
    getSecs();
    startSecs = theseSecs;
    return;
}

void end_time()
{
    getSecs();
    secs = theseSecs - startSecs;
    return;
}
