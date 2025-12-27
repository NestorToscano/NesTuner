#include "tuner.h"
#include "ssd1306.h"
#include "font5x7.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// PINS/CONSTANTS
#define SDA_PIN 10
#define SCL_PIN 11
#define I2C_PORT i2c1
#define ADC0_PIN 26

#define SAMPLE_DELAY_US 125 // ~8 kHz

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

  oled_init();

  // INITIALIZE SCREEN
  oled_clear();
  oled_draw_string(0, 0, "Sound Tuner by Nestor");
  oled_draw_string(0, FONT5X7_HEIGHT + 1, "Toscano");
  oled_draw_string(24, 32, "Note:");
  oled_flush();

  while (true) {
    uint16_t raw = adc_read();

    // slow DC tracking
    baseline += (raw - baseline) >> 7;

    int32_t signal = raw - baseline;
    int32_t amp = signal >= 0 ? signal : -signal;

    printf("raw=%u  base=%ld  sig=%ld  amp=%ld\n", raw, baseline, signal, amp);

    sleep_us(SAMPLE_DELAY_US);
  }
}