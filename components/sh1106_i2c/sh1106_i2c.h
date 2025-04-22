#ifndef SH1106_I2C_H
#define SH1106_I2C_H

#include <stdint.h>

void sh1106_init(void);
void sh1106_clear(void);
void sh1106_display(void);
void sh1106_draw_text(uint8_t x, uint8_t page, const char* text);
void sh1106_send_command(uint8_t cmd);
void sh1106_send_data(uint8_t data);
void sh1106_draw_heart(uint8_t x, uint8_t page);
void sh1106_draw_empty_heart(uint8_t x, uint8_t page);
void sh1106_draw_empty_bubbles(uint8_t x, uint8_t page);
void sh1106_draw_bubbles(uint8_t x, uint8_t page);
#endif // SH1106_I2C_H