/*
 * SampledChord.cpp
 *
 *  Created on: Mar 5, 2010
 *      Author: ross
 */

#include "SampledChord.h"

SampledChord::SampledChord() {
	// TODO Auto-generated constructor stub

}

SampledChord::~SampledChord() {
	// TODO Auto-generated destructor stub
}

void SampledChord::addSampledTone(SampledTone tone) {
	tones.push_back(tone);
}

jack_default_audio_sample_t SampledChord::getSample() {
	jack_default_audio_sample_t value = 0;

	for (unsigned int i = 0; i < tones.size(); ++i) {
		value += tones[i].getSample();
	}

	return value;
}
