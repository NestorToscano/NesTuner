#include "tuner.h"
#include <math.h>

static const char* NOTE_NAMES[12] = {
    "C", "C#", "D", "D#", "E", "F",
    "F#", "G", "G#", "A", "A#", "B"};

void freq_to_note(float freq, const char** note, int* octave, int* cents) {
  if (freq <= 0) {
    *note = "---";
    *octave = 0;
    *cents = 0;
    return;
  }
}
