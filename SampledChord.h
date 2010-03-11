/*
 * SampledChord.h
 *
 *  Created on: Mar 5, 2010
 *      Author: ross
 */

#ifndef SAMPLEDCHORD_H_
#define SAMPLEDCHORD_H_

#include <jack/jack.h>
#include <vector>
#include "SampledTone.h"

using namespace std;

class SampledChord {
public:
	SampledChord();
	virtual ~SampledChord();

	vector<SampledTone> tones;

	void addSampledTone(SampledTone tone);
	jack_default_audio_sample_t getSample();
};

#endif /* SAMPLEDCHORD_H_ */
