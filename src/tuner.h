#ifndef TUNER_H
#define TUNER_H

#include <stdint.h>

float estimate_frequency(int16_t *buf, int n, uint32_t fs);
void freq_to_note(float freq, const char **note, int *octave, int *cents);

#endif
