#include <QtGui/QApplication>
#include "mainwindow.h"
#include <jack/jack.h>
#include "Debugger.h"
#include <cstdio>
#include <sstream>
#include <cmath>
#include <vector>
#include <iostream>
#include "gnuplot-cpp/gnuplot_i.hpp"

using namespace std;

int connectToJack();
int processJackAudio(jack_nframes_t nframes, void *arg);
void jackShutdown(void *arg);
void plotSample();

jack_port_t *input_port;
jack_port_t *input_port2;
jack_port_t *output_port;
jack_port_t *output_port2;
jack_client_t *jackClient;

bool printSamples = false;
float audioFactor = 0;
vector<float> signal;
#define TOTAL_PRINTS 100
#define NUM_FRAMES 512
int numPrints = TOTAL_PRINTS;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    signal.resize(NUM_FRAMES * TOTAL_PRINTS, 0);

    connectToJack();

    int result = a.exec();

    jack_client_close(jackClient);

    return result;
}

int processJackAudio(jack_nframes_t nframes, void *arg) {

    jack_default_audio_sample_t in1_data[nframes];
    jack_default_audio_sample_t in2_data[nframes];

    jack_default_audio_sample_t *out = (jack_default_audio_sample_t *)jack_port_get_buffer(output_port, nframes);
    jack_default_audio_sample_t *in = (jack_default_audio_sample_t *)jack_port_get_buffer(input_port, nframes);

    jack_default_audio_sample_t *out2 = (jack_default_audio_sample_t *)jack_port_get_buffer(output_port2, nframes);
    jack_default_audio_sample_t *in2 = (jack_default_audio_sample_t *)jack_port_get_buffer(input_port2, nframes);

    // copy the data to our local holders
    memcpy(in1_data, in, sizeof(jack_default_audio_sample_t) * nframes);
    memcpy(in2_data, in2, sizeof(jack_default_audio_sample_t) * nframes);

    // do something cool with the data
    for (uint i = 0; i < nframes; ++i) {
        if (printSamples && numPrints > 0) {
            stringstream ss;
            ss << "frame: " << i << "\t\t" << "audio in1: " << in1_data[i];
            ss << "\t\taudio in2: " << in2_data[i] << endl;

            Debugger::getInstance().print(ss.str());
            int loc = i * (TOTAL_PRINTS - numPrints + 1);

            signal[loc] = in1_data[i];
        }


        in1_data[i] = in1_data[i] * audioFactor;

        in2_data[i] = in2_data[i] * audioFactor;

        /*
        in1_data[i] = sqrt(in1_data[i]);
        in2_data[i] = sqrt(in2_data[i]);
        */

    }
    if (numPrints == 1) {
        if (printSamples) {
            printSamples = false;
            numPrints = TOTAL_PRINTS;

            plotSample();
            //signal.resize(NUM_FRAMES * TOTAL_PRINTS, 0);

        }
    } else {
        --numPrints;
    }

    // copy the local data to the outputs
    memcpy(out, in1_data, sizeof(jack_default_audio_sample_t) * nframes);
    memcpy(out2, in2_data, sizeof(jack_default_audio_sample_t) * nframes);

    return 0;
}

void jackShutdown(void *arg) {
    // show that jack has shutdown

    Debugger::getInstance().print("disconnected from jack server\n");
}

int connectToJack() {
    /* do some cool testing of jack */
    //const char **ports;

    /* try to become a client of the JACK server */
    if ((jackClient = jack_client_new("CS450 Program")) == 0) {
        string msg = "jack server not running?\n";
        Debugger::getInstance().print(msg);
        fprintf(stderr, msg.c_str());
        return 1;
    }

    jack_set_process_callback(jackClient, processJackAudio, NULL);

    jack_on_shutdown(jackClient, jackShutdown, NULL);

    /* display the current sample rate.
    */
    stringstream ss;
    ss << "engine sample rate: " << jack_get_sample_rate(jackClient) << endl;
    Debugger::getInstance().print(ss.str());
    //printf ("engine sample rate: %" PRIu32 \n",
     //      jack_get_sample_rate (client));

    input_port = jack_port_register(jackClient, "left-in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, NULL);
    output_port = jack_port_register(jackClient, "left-out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, NULL);

    input_port2 = jack_port_register(jackClient, "right-in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, NULL);
    output_port2 = jack_port_register(jackClient, "right-out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, NULL);

    /* tell the JACK server that we are ready to roll */
    if (jack_activate(jackClient)) {
        string msg = "cannot activate client";
        fprintf(stderr, msg.c_str());
        Debugger::getInstance().print(msg);
        return 1;
    }

    /* connect the ports. Note: you can't do this before
      the client is activated, because we can't allow
      connections to be made to clients that aren't
      running.
    */

    /*
    if ((ports = jack_get_ports(jackClient, NULL, NULL, JackPortIsPhysical|JackPortIsOutput)) == NULL) {
        string msg = "Cannot find any physical capture ports\n";
        Debugger::getInstance().print(msg);
        fprintf(stderr, msg.c_str());
        return 1;
    }

    if (jack_connect(jackClient, ports[0], jack_port_name(input_port))) {

        string msg = "cannot connect input ports\n";
        fprintf(stderr, msg.c_str());
        Debugger::getInstance().print(msg);
    }

    free(ports);

    if ((ports = jack_get_ports (jackClient, NULL, NULL, JackPortIsPhysical|JackPortIsInput)) == NULL) {
        string msg = "Cannot find any physical playback ports\n";
        fprintf(stderr, msg.c_str());
        Debugger::getInstance().print(msg);
        return 1;
    }

    if (jack_connect(jackClient, jack_port_name(output_port), ports[0])) {
        string msg = "cannot connect output ports\n";
        fprintf(stderr, msg.c_str());
        Debugger::getInstance().print(msg);
    }

    free(ports);
    */

    return 0;
}

void plotSample() {
    // create a new process
    pid_t pId = fork();

    if (pId == 0) {
            // child process
        try {

            Gnuplot gplot;

            gplot.save("audiosample.ps");
            //gplot.set_style("lines");
            gplot.set_style("histograms");
            //gplot.set_samples(300
            gplot.set_xrange(0, signal.size());
            gplot.set_xlabel("frame");
            gplot.set_ylabel("amplitude");
            gplot.set_yrange(-10, 10);
            gplot.plot_x(signal);

            gplot.showonscreen();
            gplot.replot();

            // wait for input
            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail());
            cin.get();

            exit(0);
        } catch (GnuplotException ge) {
            cout << ge.what() << endl;
            exit(1);
        }
    } else {
            // parent
    }
}
