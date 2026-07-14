#include <LPC21xx.h>
#include "lcd.c"
#define mode0 0x00
#define mode1 0x08
#define mode2 0x10
#define mode3 0x18
#define mstr 5
#define SPIF_BIT 7
typedef unsigned char u8;
typedef float f32;
typedef unsigned int u32;

void spi_init()
{
	PINSEL0 = 0X1500;
	S0SPCCR=150;
	S0SPCR=(1<<mstr)|mode3;
	IODIR0 = 1<<7;
	IOPIN0 |= 1<<7;
}

u8 spi_read(u8 data)
{
	u8 stat;
	stat= S0SPSR;
	S0SPDR = data;
	while(((S0SPSR>>SPIF_BIT)&1)==0);
	return S0SPDR;
}

f32 mcp_read(u8 chno)
{
	u8 hbyte, lbyte;
	u32 adc_val=0;
	IOPIN0|=0<<7;
	spi_read(0x06);
	hbyte=spi_read(chno<<6);
	lbyte=spi_read(0x00);
	IOPIN0|=1<<7;
	adc_val=((hbyte&0xff)<<8)|lbyte;
	return (adc_val*3.3)/4096;
}

int main()
{
	f32 f;
	spi_init();
	//while(1)
	{
	f=mcp_read(0);
		Float_display(f);
	}
}

















