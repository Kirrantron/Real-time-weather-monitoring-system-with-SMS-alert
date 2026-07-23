#include <LPC21xx.h>
#include "delay.h"
#include "lcd.h"
#include "spi.h"
#include "i2c.h"
#include "uart.h"

unsigned char p[9] __attribute__((at(0x40000040))) = "";

int main(void)
{
    float f;
	//unsigned char a[]={0,35,14,5,23,6,7,26};
	//dec_to_BCD(a);
	//page_write(0x68,0x00,a,7);
   // T1PR = 15000000 - 1;

    LCD_INIT();
    uart_config();
    spi_init();
    i2c_init();

	title_display();
    while(1)
    {
        cgram_write(16);

        // Display Time 
        LCD_COMMAND(0x80);
        LCD_str("TIME: ");
        seq_read(0x68, 0x00, p, 7);
        BCD_to_dec(p);
        LCD_COMMAND(0x86);
        LCD_time(p);

        // Read Temperature 
        LCD_COMMAND(0xC0);
        LCD_str("Temp: ");
        f = mcp_read(0) * 100;
        LCD_COMMAND(0xC6);
        LCD_float(f);
        LCD_COMMAND(0xCA);
        LCD_DATA(0);
        LCD_DATA('C');
						
        // Timer Reset Check
        if(T1TC > 3600)
        {
            T1TCR = 0x03;
        }

        // High Temperature Alert
        if(f > 33.0 && T1TC == 0)
        {
            T1TCR = 0x01;
            UART0_msg(f);
        }					
    }
}
