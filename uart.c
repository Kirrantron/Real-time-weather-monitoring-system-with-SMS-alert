#include <LPC21xx.h>
#include "uart.h"
#include "delay.h"

void uart_config(void)
{
    PINSEL0 |= 0x5;
    U0LCR = 0x83;
    U0DLL = 97;
    U0DLM = 0;
    U0LCR = 0x03;
}

void UART0_TX(unsigned char txbyte)
{
    while(((U0LSR >> 5) & 1) == 0);
    U0THR = txbyte;
}

void UART0_STR(unsigned char *s)
{
    while(*s)
        UART0_TX(*s++);
}

void UART0_int(int n)
{
    signed char arr[5];
    signed char i = 0;
    if(n == 0)
    {
        UART0_TX('0');
    }
    else
    {
        if(n < 0)
        {
            UART0_TX('-');
            n = -n;
        }
        while(n > 0)
        {
            arr[i++] = n % 10;
            n = n / 10;
        }
        for(--i; i >= 0; i--)
            UART0_TX(arr[i] + 48);
    }
}

void UART0_float(float f)
{
    int temp = f;
    UART0_int(temp);
    UART0_TX('.');
    temp = (f - temp) * 10;
    UART0_int(temp);
}

void UART0_msg(float f)
{
    UART0_STR("AT\r\n");
    delay(250);
    UART0_STR("AT+CMGF=1\r\n");
    delay(250);
    UART0_STR("AT+CMGS=\"+916381953071\"\r\n");
    delay(250);
    UART0_STR("Temp is ");
    delay(250);
    UART0_float(f);
    delay(250);
    UART0_STR("degree celsius \r\nDay is too hot!!!\r\nTake care \r\n");
    UART0_TX(0x1A);
}
