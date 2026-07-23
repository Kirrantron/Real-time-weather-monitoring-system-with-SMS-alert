#include <LPC21xx.h>
#include "delay.h"

void delay(unsigned int n)
{
    T0PR = 15000 - 1;
    T0TCR = 0x01;
    while(T0TC < n);
    T0TCR = 0x03;
    T0TCR = 0x00;
}

void delay_msg(void)
{
    T1PR = 15000000 - 1;
    T1TCR = 0x01;
    while(T1TC < 3600);
    T1TCR = 0x03;
    T1TCR = 0x00;
}
