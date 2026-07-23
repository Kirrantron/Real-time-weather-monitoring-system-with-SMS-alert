#ifndef UART_H
#define UART_H

void uart_config(void);
void UART0_TX(unsigned char txbyte);
void UART0_STR(unsigned char *s);
void UART0_msg(float f);
void UART0_float(float f);
void UART0_int(int n);

#endif
