#ifndef SPI_H
#define SPI_H

#define mode0 0x00
#define mode1 0x08
#define mode2 0x10
#define mode3 0x18
#define mstr 5
#define SPIF_BIT 7

typedef unsigned char u8;
typedef float f32;
typedef unsigned int u32;

void spi_init(void);
u8 spi_read(u8 data);
f32 mcp_read(u8 chno);

#endif
