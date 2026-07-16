#include <LPC21xx.h>

/* ==========================================
   1. LCD DEFINITIONS & CONFIGURATIONS
   ========================================== */
#define LCD_D 0xF << 8
#define RS    1 << 12
#define E     1 << 13

unsigned char cgram_lut[]={0x04,0x0a,0x11,0x0a,0x04,0x00,0x00,0x00,
0X1F,0X11,0X11,0X11,0X1F,0X00,0X00,0X00};

void LCD_INIT(void);
void LCD_COMMAND(unsigned char cmd);
void LCD_DATA(unsigned char d);
void LCD_int(int n);
void LCD_float(float f);
void LCD_str(unsigned char *s);
void LCD_time(unsigned char *s);
void cgram_write(unsigned char n);

/* ==========================================
   2. SPI DEFINITIONS & CONFIGURATIONS
   ========================================== */
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

/* ==========================================
   3. I2C DEFINITIONS & CONFIGURATIONS
   ========================================== */
#define scl 1<<4
#define sda 1<<6
#define aa 1<<2
#define si 3
#define sto 1<<4
#define sta 1<<5
#define i2en 1<<6
#define loadval 75
#define led0 1<<0
#define led1 1<<1

void i2c_init(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_write(unsigned char dat);
void i2c_stop(void);
unsigned char i2c_nack(void);
unsigned char i2c_ack(void);
void eeprom_write(unsigned char slave, unsigned char buff, unsigned char dat);
unsigned char eeprom_read(unsigned char slave, unsigned char buff);
void page_write(unsigned char slave, unsigned char buff, unsigned char *s, unsigned char nBytes);
void seq_read(unsigned char slave, unsigned char buff, unsigned char *p, unsigned char nBytes);
void dec_to_BCD(unsigned char *s);
void BCD_to_dec(unsigned char *s);

/* ==========================================
   4. UART DEFINITIONS & CONFIGURATIONS
   ========================================== */
void uart_config(void);
void UART0_TX(unsigned char txbyte);
void UART0_STR(unsigned char *s);
void UART0_msg(float f);
void UART0_float(float f);
void UART0_int(int n);

/* ==========================================
   5. COMMON UTILITIES
   ========================================== */
void delay(unsigned int n);
void delay_msg(void);


/* ==========================================
   FUNCTION IMPLEMENTATIONS
   ========================================== */

// Shared System Delay Logic
void delay(unsigned int n)
{
	T0PR = 15000 - 1;
	T0TCR = 0X01;
	while(T0TC < n);
	T0TCR = 0X03;
	T0TCR = 0X00;
}

void delay_msg()
{
	T1PR = 15000000 - 1;
	T1TCR = 0X01;
	while(T1TC < 3600);
	T1TCR = 0X03;
	T1TCR = 0X00;
}

// LCD Functions
void LCD_INIT(void)
{
	IODIR0 |= LCD_D | RS | E; 
	LCD_COMMAND(0X01); 
	LCD_COMMAND(0X02); 
	LCD_COMMAND(0X0C); 
	LCD_COMMAND(0X28); 
}

void LCD_COMMAND(unsigned char cmd)
{
	IOCLR0 = LCD_D;
	IOSET0 = (cmd & 0xf0) << 4;
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
	IOSET0 = (d & 0xf0) << 4;
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
	signed char i=0;
	if(n==0)
		LCD_DATA('0');
	else
	{
		if(n<0)
		{
			LCD_DATA('-');
			n=-n;
		}
		while(n>0)
		{
			arr[i++]=n%10;
			n=n/10;
		}
		for(--i;i>=0;i--)
		LCD_DATA(arr[i]+48);
	}
}

void LCD_float(float f)
{
	int temp;
	temp=f;
	LCD_int(temp); 
	LCD_DATA('.');
	temp=(f-temp)*10;
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
LCD_COMMAND(0X40);
for(i=0;i<n;i++)
LCD_DATA(cgram_lut[i]);
}

// SPI Functions
void spi_init(void)
{
	PINSEL0 |= 0X1500;
	S0SPCCR = 150;
	S0SPCR = (1 << mstr) | mode3;
	IODIR0 |= 1 << 7;
	IOPIN0 |= 1 << 7;
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
	IOCLR0 = 1<<7;
	spi_read(0x06);
	hbyte = spi_read(chno << 6);
	lbyte = spi_read(0x00);
	IOSET0 = 1 << 7;
	adc_val = ((hbyte & 0xff) << 8) | lbyte;
	return (adc_val * 3.3) / 4096;
}

// I2C Functions
void i2c_init(void)
{
	PINSEL0 |= scl | sda;
	I2SCLL = loadval;
	I2SCLH = loadval;
	I2CONSET = i2en;
}

void i2c_start(void)
{
	I2CONSET = sta;
	while(((I2CONSET >> si) & 1) == 0);
	I2CONCLR = sta;
}

void i2c_restart(void)
{
	I2CONSET = sta;
	I2CONCLR = 1 << si;
	while(((I2CONSET >> si) & 1) == 0);
	I2CONCLR = sta;
}

void i2c_write(unsigned char dat)
{
	I2DAT = dat;
	I2CONCLR = 1 << si;
	while(((I2CONSET >> si) & 1) == 0);
}

void i2c_stop(void)
{
	I2CONSET = sto;
	I2CONCLR = 1 << si;
}

unsigned char i2c_nack(void)
{
	I2CONSET = 0x00;
	I2CONCLR = 1 << si;
	while(((I2CONSET >> si) & 1) == 0);
	return I2DAT;
}

unsigned char i2c_ack(void)
{
	I2CONSET = aa;
	I2CONCLR = 1 << si;
	while(((I2CONSET >> si) & 1) == 0);
	I2CONCLR = aa;
	return I2DAT;
}

void eeprom_write(unsigned char slave, unsigned char buff, unsigned char dat)
{
	i2c_start();
	i2c_write(slave << 1);
	i2c_write(buff);
	i2c_write(dat);
	i2c_stop();
	delay(10);
}

unsigned char eeprom_read(unsigned char slave, unsigned char buff)
{
	unsigned char dat;
	i2c_start();
	i2c_write(slave << 1);
	i2c_write(buff);
	i2c_restart();
	i2c_write((slave << 1) | 1);
	dat = i2c_nack();
	i2c_stop();
	return dat;
}

void page_write(unsigned char slave, unsigned char buff, unsigned char *s, unsigned char nBytes)
{
	unsigned char i;
	i2c_start();
	i2c_write(slave << 1);
	i2c_write(buff);
	for(i = 0; i < nBytes; i++)
	{
		i2c_write(s[i]);
	}
	i2c_stop();
	delay(10);
}

void seq_read(unsigned char slave, unsigned char buff, unsigned char *p, unsigned char nBytes)
{
	unsigned char i;
	i2c_start();
	i2c_write(slave << 1);
	i2c_write(buff);
	i2c_restart();
	i2c_write((slave << 1) | 1);
	for(i = 0; i < nBytes - 1; i++)
	{
		p[i] = i2c_ack();
	}
	p[i] = i2c_nack();
	i2c_stop();
	delay(10);
}

void dec_to_BCD(unsigned char *s)
{
	unsigned char i, a, b;
	for(i = 0; i < 7; i++)
	{
		a = s[i] % 10;
		b = s[i] / 10;
		s[i] = (b << 4) + a;
	}
}

void BCD_to_dec(unsigned char *s)
{
	unsigned char i, a, b;
	for(i = 0; i < 7; i++)
	{
		a = s[i] & 0x0f;
		b = (s[i] >> 4) & 0x0f;
		s[i] = b * 10 + a;
	}
}

// UART Functions
void uart_config(void)
{
	PINSEL0 |= 0X5;
	U0LCR = 0X83;
	U0DLL = 97;
	U0DLM = 0;
	U0LCR = 0X03;
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
	signed char i=0;
	if(n==0)
		UART0_TX('0');
	else
	{
		if(n<0)
		{
			UART0_TX('-');
			n=-n;
		}
		while(n>0)
		{
			arr[i++]=n%10;
			n=n/10;
		}
		for(--i;i>=0;i--)
		UART0_TX(arr[i]+48);
	}
}

void UART0_float(float f)
{
	int temp;
	temp=f;
	UART0_int(temp); 
	UART0_TX('.');
	temp=(f-temp)*10;
	UART0_int(temp); 
}


void UART0_msg(float f)
{
	UART0_STR("AT\r\n");
	delay(250);
	UART0_STR("AT+CMGF=1\r\n");
	delay(250);
	UART0_STR("AT+CMGS=\"<+91 ph.no>\"\r\n");
	delay(250);                                                                                                     
	UART0_STR("Temp is ");
	delay(250);
	UART0_float(f);
	delay(250);
	UART0_STR("degree celsius \r\nDay is too hot!!!\r\nTake care \r\n");
	UART0_TX(0X1A);
}

/* ==========================================
   6. MAIN EXECUTION BLOCK
   ========================================== */
unsigned char p[9] __attribute__((at(0x40000040)))="";


int main()
{
	int i;
	float f;
	T1PR=15000000-1;
	LCD_INIT();
	uart_config();
	spi_init();
	i2c_init();
	
		/**LCD_str("     REAL TIME WEATHER MONITORING SYSTEM");
		
	for(i=0; i<40; i++)
	{
		LCD_COMMAND(0X18);
		delay(150);
	}**/
	
	while(1)
	{
		cgram_write(16);
		LCD_COMMAND(0X80);
		LCD_str("TIME: ");
		seq_read(0x68, 0x00, p, 7);
		BCD_to_dec(p);
		LCD_COMMAND(0X86);
		LCD_time(p);
		LCD_COMMAND(0XC0);
		LCD_str("Temp: ");
		f = mcp_read(0)*100;
		LCD_COMMAND(0XC6);
		LCD_float(f);
		LCD_COMMAND(0XCA);
		LCD_DATA(1);
		LCD_DATA('C');
		if(T1TC>3600)
		{
			T1TCR=0X03;
		}
		if(f > 35.0 && T1TC == 0)
		{
			T1TCR=0X01;
			UART0_msg(f);
		}
	}
}























