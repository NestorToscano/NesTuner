#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include <stdbool.h>

// Display Size
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_PAGES (SSD1306_HEIGHT / 8)

// OLED API
void oled_init(void);
void oled_clear(void);
void oled_flush(void);
void oled_set_pixel(int x, int y, bool on);
void oled_draw_char(int x, int y, char c);
void oled_draw_string(int x, int y, const char *s);
void oled_clear_rect(int x, int y, int w, int h);

#endif
