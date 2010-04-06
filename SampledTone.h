/*
 * SampledTone.h
 *
 *  Created on: Mar 5, 2010
 *      Author: ross
 */

#ifndef SAMPLEDTONE_H_
#define SAMPLEDTONE_H_

#define SAMPLEDTONE_PI 3.14159265f
#define PRACTICALLY_ZERO(d) (d < 0.0000000001 && d > -0.0000000001)

#include <jack/jack.h>

class SampledTone {
public:
	SampledTone(jack_default_audio_sample_t freq, int framesPerSecond, float _amplitude);
	SampledTone();
	virtual ~SampledTone();

	jack_default_audio_sample_t frequency;
	int sampleRate;
	float amplitude;
	int sampleCount;
	jack_default_audio_sample_t framesPerCycle;
	int cycleEndSample;

	jack_default_audio_sample_t getSample();
	void init(jack_default_audio_sample_t freq, int framesPerSecond);
};

#endif /* SAMPLEDTONE_H_ */
