#include "ssd1306.h"
#include "font5x7.h"
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Hardware Configs
#define I2C_PORT i2c1
#define OLED_ADDR 0x3C

// SSD1306 Commands
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF

#define SSD1306_SET_MEMORY_MODE 0x20
#define SSD1306_MEMORY_HORIZONTAL 0x00

#define SSD1306_SET_PAGE_START 0xB0

#define SSD1306_COM_SCAN_DEC 0xC8

#define SSD1306_SET_LOW_COLUMN 0x00
#define SSD1306_SET_HIGH_COLUMN 0x10

#define SSD1306_SET_START_LINE 0x40

#define SSD1306_SET_CONTRAST 0x81

#define SSD1306_SEG_REMAP_FLIPPED 0xA1
#define SSD1306_NORMAL_DISPLAY 0xA6

#define SSD1306_SET_MULTIPLEX 0xA8
#define SSD1306_DISPLAY_ALL_ON_RESUME 0xA4
#define SSD1306_SET_DISPLAY_OFFSET 0xD3
#define SSD1306_SET_CLOCK_DIV 0xD5
#define SSD1306_SET_PRECHARGE 0xD9
#define SSD1306_SET_COM_PINS 0xDA
#define SSD1306_SET_VCOM_DETECT 0xDB
#define SSD1306_CHARGE_PUMP 0x8D
#define SSD1306_CHARGE_PUMP_ENABLE 0x14

static uint8_t fb[SSD1306_WIDTH * SSD1306_PAGES];


// I2C Low-level commands
static void oled_cmd(uint8_t cmd) {
  uint8_t buf[2] = {0x00, cmd};
  i2c_write_blocking(I2C_PORT, OLED_ADDR, buf, 2, false);
}

static void oled_data(const uint8_t *data, size_t len) {
  uint8_t buf[len + 1]; // control + 128 bytes wide 
  buf[0] = 0x40;
  memcpy(&buf[1], data, len); // push 128 byte page starting at 1
  i2c_write_blocking(I2C_PORT, OLED_ADDR, buf, len + 1, false);
}

// API

void oled_init(void) {
  sleep_ms(100);

  oled_cmd(SSD1306_DISPLAY_OFF);

  oled_cmd(SSD1306_SET_MEMORY_MODE);
  oled_cmd(SSD1306_MEMORY_HORIZONTAL);

  oled_cmd(SSD1306_SET_PAGE_START);
  oled_cmd(SSD1306_COM_SCAN_DEC);

  oled_cmd(SSD1306_SET_LOW_COLUMN);
  oled_cmd(SSD1306_SET_HIGH_COLUMN);

  oled_cmd(SSD1306_SET_START_LINE | 0x00);

  oled_cmd(SSD1306_SET_CONTRAST);
  oled_cmd(0x7F);

  oled_cmd(SSD1306_SEG_REMAP_FLIPPED);
  oled_cmd(SSD1306_NORMAL_DISPLAY);

  oled_cmd(SSD1306_SET_MULTIPLEX);
  oled_cmd(0x3F);

  oled_cmd(SSD1306_DISPLAY_ALL_ON_RESUME);

  oled_cmd(SSD1306_SET_DISPLAY_OFFSET);
  oled_cmd(0x00);

  oled_cmd(SSD1306_SET_CLOCK_DIV);
  oled_cmd(0x80);

  oled_cmd(SSD1306_SET_PRECHARGE);
  oled_cmd(0xF1);

  oled_cmd(SSD1306_SET_COM_PINS);
  oled_cmd(0x12);

  oled_cmd(SSD1306_SET_VCOM_DETECT);
  oled_cmd(0x40);

  oled_cmd(SSD1306_CHARGE_PUMP);
  oled_cmd(SSD1306_CHARGE_PUMP_ENABLE);

  oled_cmd(SSD1306_DISPLAY_ON);
}

void oled_clear(void) {
  memset(fb, 0, sizeof(fb));
}

void oled_flush(void) { // push each page in fb into hardware
  for (int page = 0; page < SSD1306_PAGES; page++) {
    oled_cmd(SSD1306_SET_PAGE_START + page); // 0xB 0 + page
    oled_cmd(SSD1306_SET_LOW_COLUMN); // 0
    oled_cmd(SSD1306_SET_HIGH_COLUMN); // 8
    oled_data(&fb[page * SSD1306_WIDTH], SSD1306_WIDTH);
  }
}

void oled_set_pixel(int x, int y, bool on) {
  if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) {
    return;
  }
  int page = y / 8;
  int index = page * SSD1306_WIDTH + x;

  int bit = y % 8;

  if (on) // turn off / on 
    fb[index] |= (1 << bit);
  else
    fb[index] &= ~(1 << bit);;
}

void oled_draw_char(int x, int y, char c) {
  if (c < 32 || c > 127)
    return;

  const uint8_t *glyph = font5x7[c - 32];

  for (int col = 0; col < FONT5X7_WIDTH; col++) {
    uint8_t bits = glyph[col];
    for (int row = 0; row < FONT5X7_HEIGHT; row++) {
      if (bits & (1 << row)) // test each bit in that column
        oled_set_pixel(x + col, y + row, true);
    }
  }
}

void oled_draw_string(int x, int y, const char *s)
{
  int cursor_x = x;

  while (*s)
  {
    oled_draw_char(cursor_x, y, *s);
    cursor_x += FONT5X7_WIDTH + 1; // 1 pixel spacing
    s++;
  }
}

void oled_clear_rect(int x, int y, int w, int h)
{
  for (int yy = y; yy < y + h; yy++)
  {
    for (int xx = x; xx < x + w; xx++)
    {
      oled_set_pixel(xx, yy, false);
    }
  }
}