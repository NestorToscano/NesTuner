#include "tuner.h"
#include "ssd1306.h"
#include "font5x7.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// PINS/CONSTANTS
#define SDA_PIN 10
#define SCL_PIN 11
#define I2C_PORT i2c1

#define ADC0_PIN 26

#define BUF_SIZE 1024       // samples per estimate
#define SAMPLE_DELAY_US 125 // ~8 kHz
#define FS (1000000 / SAMPLE_DELAY_US)

int main(void) {
  stdio_init_all();

  // ADC INPUT / INITIALIZATION
  adc_init();
  adc_gpio_init(ADC0_PIN);
  adc_select_input(0);

  int32_t baseline = 2048;

  // I2C OLED INPUT
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SDA_PIN);
  gpio_pull_up(SCL_PIN);

  // INITIALIZE SCREEN
  oled_init();
  oled_clear();
  oled_draw_string(0, 0, "Sound Tuner - Nestor");
  oled_draw_string(0, 10, "---------------------");
  oled_draw_string(0, 22, "Freq:");
  oled_draw_string(0, 32, "Note:");
  oled_draw_string(0, 42, "Cents:");
  oled_draw_string(0, 52, "Tune:");
  oled_flush();

  static int16_t samples[BUF_SIZE];

  while (true) {

    // Acquire samples
    for (int i = 0; i < BUF_SIZE; i++)
    {
      uint16_t raw = adc_read();
      baseline += (raw - baseline) >> 7;
      samples[i] = (int16_t)((int32_t)raw - baseline);
      sleep_us(SAMPLE_DELAY_US);
    }

    // Estimate pitch
    float freq = estimate_frequency(samples, BUF_SIZE, FS);

    // Convert to actual note
    const char *note;
    int octave, cents;
    freq_to_note(freq, &note, &octave, &cents);

    // partial updates
    oled_clear_rect(40, 22, 80, 7); // freq
    oled_clear_rect(40, 32, 80, 7); // note
    oled_clear_rect(40, 42, 80, 7); // cents
    oled_clear_rect(40, 52, 80, 7); // indicator

    if (freq < 80.0f) {
      oled_draw_string(40, 22, "Listening ...");
    }
    else {
      char buf[32];

      snprintf(buf, sizeof(buf), "%.1f Hz", freq);
      oled_draw_string(40, 22, buf);

      snprintf(buf, sizeof(buf), "%s%d", note, octave);
      oled_draw_string(40, 32, buf);

      snprintf(buf, sizeof(buf), "%+d", cents);
      oled_draw_string(40, 42, buf);

      if (cents > 5)
        oled_draw_string(40, 52, "Sharp  >");
      else if (cents < -5)
        oled_draw_string(40, 52, "Flat   <");
      else
        oled_draw_string(40, 52, "In Tune |");
    }

    oled_flush();

    // Debugging
    // printf("Freq=%.1f  Note=%s%d  Cents=%+d\n", freq, note, octave, cents);
  }

  return 0;
}