#include <QtGui/QApplication>
#include "mainwindow.h"
#include "Debugger.h"
#include <cstdio>
#include <sstream>
#include <cmath>
#include <vector>
#include <iostream>
#include "gnuplot_i.h"
#include <jack/jack.h>
#include "SampledTone.h"
#include "SampledChord.h"

#define A4 440.0f
//#define PI 3.14159265f
//#define D4 293.66f
#define D4 A4 * (2.0f / 3.0f)
//#define F_SHARP4 369.99f
#define F_SHARP4 D4 * (5.0f / 4.0f)

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

//bool printSamples = false;
//float audioFactor = 0;
//vector<float> signal;
//#define TOTAL_PRINTS 100
//#define NUM_FRAMES 512
//int numPrints = TOTAL_PRINTS;
//int numFrames = 0;
int sampleRate = 0;
//float frequency = 440.0f;
//int sampleCount = 0;
//double percentOfSecond = 0.0;

SampledTone root, third, fourth, fifth, lowRoot;
SampledChord chord;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //signal.resize(NUM_FRAMES * TOTAL_PRINTS, 0);

    connectToJack();

    int result = a.exec();

    jack_client_close(jackClient);

    return result;
}

int processJackAudio(jack_nframes_t nframes, void *arg) {
	//numFrames = nframes;

    //jack_default_audio_sample_t in1_data[nframes];
    //jack_default_audio_sample_t in2_data[nframes];

    jack_default_audio_sample_t *out1 = (jack_default_audio_sample_t*)jack_port_get_buffer(output_port, nframes);
    //jack_default_audio_sample_t *in1 = (jack_default_audio_sample_t*)jack_port_get_buffer(input_port, nframes);

    jack_default_audio_sample_t *out2 = (jack_default_audio_sample_t*)jack_port_get_buffer(output_port2, nframes);
    //jack_default_audio_sample_t *in2 = (jack_default_audio_sample_t*)jack_port_get_buffer(input_port2, nframes);

    //percentOfSecond = sampleCount / sampleRate;

	//jack_default_audio_sample_t *out1ptr = out1;
	//jack_default_audio_sample_t *out2ptr = out2;

	//int factor = 2 * PI * frequency * sampleCount / sampleRate;
    //jack_default_audio_sample_t amplitude = 5.0;

	//float factor = 2.0f * PI * frequency;

	//jack_default_audio_sample_t divisor = sampleRate * nframes;

    for (jack_nframes_t i = 0; i < nframes; ++i) {
    	out1[i] = 0;
    	out2[i] = 0;

    	jack_default_audio_sample_t _chord = chord.getSample();
    	out1[i] += _chord;
		out2[i] += _chord;

		/*

	    out1[i] += root.getSample();
    	out2[i] += fifth.getSample();
    	*/

    	/*
    	jack_default_audio_sample_t _third = third.getSample();
    	out1[i] += 0.25f * _third;
		out2[i] += 0.25f * _third;
		*/

		/*
    	jack_default_audio_sample_t _fourth = fourth.getSample();
    	out1[i] += _fourth;
		out2[i] +=  _fourth;
		*/

		/*
		jack_default_audio_sample_t _lowRoot = lowRoot.getSample();
		out1[i] += 0.4f * _lowRoot;
		out2[i] += 0.4f * _lowRoot;
		*/

    	/*
	    ++sampleCount;
	    if (sampleCount == sampleRate) {
	    	sampleCount = 0;
	    }
	    */
    }

    // generate sound data

    /*
    // copy the data to our local holders
    memcpy(in1_data, in1, sizeof(jack_default_audio_sample_t) * nframes);
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

    }
    if (numPrints == 1) {
        if (printSamples) {
            printSamples = false;
            numPrints = TOTAL_PRINTS;

            //plotSample();
            //signal.resize(NUM_FRAMES * TOTAL_PRINTS, 0);

        }
    } else {
        --numPrints;
    }

    // copy the local data to the outputs
    memcpy(out1, in1_data, sizeof(jack_default_audio_sample_t) * nframes);
    memcpy(out2, in2_data, sizeof(jack_default_audio_sample_t) * nframes);
    */

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

    sampleRate = jack_get_sample_rate(jackClient);
    //sampleCount = 0;
    /* display the current sample rate.
    */
    stringstream ss;
    ss << "engine sample rate: " << sampleRate << endl;
    Debugger::getInstance().print(ss.str());
    //printf ("engine sample rate: %" PRIu32 \n",
     //      jack_get_sample_rate (client));

    input_port = jack_port_register(jackClient, "left-in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, NULL);
    output_port = jack_port_register(jackClient, "left-out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, NULL);

    input_port2 = jack_port_register(jackClient, "right-in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, NULL);
    output_port2 = jack_port_register(jackClient, "right-out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, NULL);

    root = SampledTone(D4, sampleRate, 0.8f);
    third = SampledTone(F_SHARP4, sampleRate, 0.25f);
    fourth = SampledTone(D4 * (4.0f / 3.0f), sampleRate, 0.3f);
    fifth = SampledTone(A4, sampleRate, 0.8f);
    lowRoot = SampledTone(A4 / 8.0f, sampleRate, 0.1f);

    chord.addSampledTone(SampledTone(D4, sampleRate, 0.6f));
    chord.addSampledTone(SampledTone(D4 * (4.0f / 3.0f), sampleRate, 0.2f));
    chord.addSampledTone(SampledTone(A4, sampleRate, 0.5f));

    chord.addSampledTone(SampledTone(D4 * 2.0f, sampleRate, 0.075f));
    chord.addSampledTone(SampledTone(A4 * 2.0f, sampleRate, 0.05f));

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
