#ifndef LCD_H
#define LCD_H

#define LCD_D (0xF << 8)
#define RS    (1 << 12)
#define E     (1 << 13)

void LCD_INIT(void);
void LCD_COMMAND(unsigned char cmd);
void LCD_DATA(unsigned char d);
void LCD_int(int n);
void LCD_float(float f);
void LCD_str(unsigned char *s);
void LCD_time(unsigned char *s);
void cgram_write(unsigned char n);
void title_display(void);

#endif
