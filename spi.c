#include <LPC21xx.h>
#include "spi.h"

void spi_init(void)
{
    PINSEL0 |= 0x1500;
    S0SPCCR = 150;
    S0SPCR = (1 << mstr) | mode3;
    IODIR0 |= (1 << 7);
    IOPIN0 |= (1 << 7);
}

u8 spi_read(u8 data)
{
    u8 stat;
    stat = S0SPSR;
    S0SPDR = data;
    while(((S0SPSR >> SPIF_BIT) & 1) == 0);
    return S0SPDR;
}

f32 mcp_read(u8 chno)
{
    u8 hbyte, lbyte;
    u32 adc_val = 0;
    IOPIN0 = 0 << 7;
    spi_read(0x06);
    hbyte = spi_read(chno << 6);
    lbyte = spi_read(0x00);
    IOPIN0 = 1 << 7;
    adc_val = ((hbyte & 0x0F) << 8) | lbyte;
    return (adc_val * 3.3) / 4096;
}
