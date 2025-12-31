#include "tuner.h"
#include <math.h>

static const char *NOTE_NAMES[12] = {
    "C", "C#", "D", "D#", "E", "F",
    "F#", "G", "G#", "A", "A#", "B"};

    
void freq_to_note(float freq, const char **note, int *octave, int *cents) {
  if (freq <= 0.0f) {
    *note = "---";
    *octave = 0;
    *cents = 0;
    return;
  }

  // MIDI note 69 = A4 = 440 Hz
  float midi = 69.0f + 12.0f * log2f(freq / 440.0f);

  // nearest whole note
  int midi_int = (int)roundf(midi);

  int note_index = midi_int % 12;
  if (note_index < 0)
    note_index += 12;

  int oct = (midi_int / 12) - 1; // MIDI octave rule

  // cents = 1200 * log2(freq / note_freq)
  float target_freq = 440.0f * powf(2.0f, (midi_int - 69) / 12.0f);
  float cent = 1200.0f * log2f(freq / target_freq);

  *note = NOTE_NAMES[note_index];
  *octave = oct;
  *cents = (int)roundf(cent);
}

float estimate_frequency(int16_t *buf, int n, uint32_t fs) {
  int crossings = 0;
  for (int i = 1; i < n; i++) {
    if ((buf[i - 1] < 0 && buf[i] >= 0) || (buf[i - 1] > 0 && buf[i] <= 0)) {
      crossings++;
    }
  }
  if (crossings < 2) return 0.0f;
  float period = (float) n / (float) crossings;
  return fs / period;
}
