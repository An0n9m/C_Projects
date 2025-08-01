#ifndef ST7920LIB_H
#define ST7920LIB_H

     // Clock frequency
     #define F_CPU 16000000UL
     #include <stdint.h>

     // Control pins (modify as needed)
     #define RS PB0
     #define RW PB1
     #define E  PB2
     #define RE PB3

     // Control macros
     #define RS_HIGH() (PORTB |= (1 << RS))
     #define RS_LOW()  (PORTB &= ~(1 << RS))
     #define RW_HIGH() (PORTB |= (1 << RW))
     #define RW_LOW()  (PORTB &= ~(1 << RW))
     #define E_HIGH()  (PORTB |= (1 << E))
     #define E_LOW()   (PORTB &= ~(1 << E))
     #define RE_HIGH() (PORTB |= (1 << RE))
     #define RE_LOW()  (PORTB &= ~(1 << RE))

     // Data port macro (modify as needed)
     #define LCD_DATA_OUT(x) (PORTD = (x))

     // Function prototypes
     void lcd_init(void);
     void lcd_command(uint8_t cmd);
     void lcd_data(uint8_t data);
     void lcd_pulse_enable(void);
     void fill_buffer(uint8_t x, uint8_t y);
     void clear_buffer(uint8_t x, uint8_t y);
     void clear_framebuffer(void);
     void lcd_draw_framebuffer(void);
     void set_pixel(uint8_t x, uint8_t y);
     void clear_pixel(uint8_t x, uint8_t y);
     void display_init(void);
     void display_command(uint8_t cmd);
     void display_data(uint8_t data);
     void display_address(uint8_t x, uint8_t y);
     void graphical_init(void);
     void DDRAM_test(void);
     void GDRAM_test(void);
     void DDRAM_and_GDRAM_test(void);
     void pong_test(void);
// written by an0n9m (Davit Jalagonia)
#endif
