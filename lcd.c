#include <LPC21xx.h>
#include "lcd.h"
#include "delay.h"

static unsigned char cgram_lut[] = {
    0x04, 0x0a, 0x11, 0x0a, 0x04, 0x00, 0x00, 0x00,
    0x1F, 0x11, 0x11, 0x11, 0x1F, 0x00, 0x00, 0x00
};

void LCD_INIT(void)
{
    IODIR0 |= LCD_D | RS | E;
    LCD_COMMAND(0x01);
    LCD_COMMAND(0x02);
    LCD_COMMAND(0x0C);
    LCD_COMMAND(0x28);
}

void LCD_COMMAND(unsigned char cmd)
{
    IOCLR0 = LCD_D;
    IOSET0 = (cmd & 0xF0) << 4;
    IOCLR0 = RS;
    IOSET0 = E;
    delay(2);
    IOCLR0 = E;

    IOCLR0 = LCD_D;
    IOSET0 = (cmd & 0x0F) << 8;
    IOCLR0 = RS;
    IOSET0 = E;
    delay(2);
    IOCLR0 = E;
}

void LCD_DATA(unsigned char d)
{
    IOCLR0 = LCD_D;
    IOSET0 = (d & 0xF0) << 4;
    IOSET0 = RS;
    IOSET0 = E;
    delay(2);
    IOCLR0 = E;

    IOCLR0 = LCD_D;
    IOSET0 = (d & 0x0F) << 8;
    IOSET0 = RS;
    IOSET0 = E;
    delay(2);
    IOCLR0 = E;
}

void LCD_int(int n)
{
    signed char arr[5];
    signed char i = 0;
    if(n == 0)
    {
        LCD_DATA('0');
    }
    else
    {
        if(n < 0)
        {
            LCD_DATA('-');
            n = -n;
        }
        while(n > 0)
        {
            arr[i++] = n % 10;
            n = n / 10;
        }
        for(--i; i >= 0; i--)
            LCD_DATA(arr[i] + 48);
    }
}

void LCD_float(float f)
{
    int temp = f;
    LCD_int(temp);
    LCD_DATA('.');
    temp = (f - temp) * 10;
    LCD_int(temp);
}

void LCD_str(unsigned char *s)
{
    while(*s)
    {
        LCD_DATA(*s++);
    }
}

void LCD_time(unsigned char *s)
{
    LCD_int(s[2]);
    LCD_DATA(':');
    LCD_int(s[1]);
}

void cgram_write(unsigned char n)
{
    unsigned char i;
    LCD_COMMAND(0x40);
    for(i = 0; i < n; i++)
        LCD_DATA(cgram_lut[i]);
}

void title_display()
{
	unsigned char i;
	LCD_str("      Realtime Weather Monitoring System");
	for(i=0;i<25;i++)
	{
		LCD_COMMAND(0X18);
		delay(250);
	}
	LCD_COMMAND(0X01);
}









