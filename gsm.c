#include<LPC21xx.h>

void uart_config()
{
PINSEL0 |=0X5;
U0LCR = 0X83;
U0DLL= 97;
U0DLM=0;
U0LCR=0X03;

}

void UART0_TX(unsigned char txbyte)
{
while(((U0LSR>>5)&1)==0);
U0THR=txbyte;
}

void UART0_STR(unsigned char *s)
{
while(*s)
UART0_TX(*s++);
}

void delay(unsigned int n)
{
T0PR=15000-1;
T0TCR=0X01;
while(T0TC<n);
T0TCR=0X02;
T0TCR=0X00;
}

int main()
{
uart_config();
UART0_STR("AT\r\n");
delay(500);
UART0_STR("AT+CMGF=1\r\n");
delay(500);
UART0_STR("AT+CMGS=\"+916381953071\"\r\n");
delay(500);																																	 
UART0_STR("THE WEATHER WILL BE TOO HOT \nTAKE CARE\r\n");
delay(500);
UART0_TX(0X1A);
}