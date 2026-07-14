#include<LPC21XX.H>
#include<string.h>
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

void i2c_init()
{
	PINSEL0 = scl|sda;
	I2SCLL=loadval;
	I2SCLH = loadval;
	I2CONSET = i2en;
}

void i2c_start()
{
	I2CONSET=sta;
	while(((I2CONSET>>si)&1)==0);
	I2CONCLR=sta;
}

void i2c_restart()
{
	I2CONSET=sta;
	I2CONCLR=1<<si;
	while(((I2CONSET>>si)&1)==0);
	I2CONCLR=sta;
}

void i2c_write(unsigned char dat)
{
	I2DAT=dat;
	I2CONCLR=1<<si;
	while(((I2CONSET>>si)&1)==0);
}

void i2c_stop()
{
	I2CONSET=sto;
	I2CONCLR=1<<si;
}

unsigned char i2c_nack()
{
	I2CONSET = 0x00;
	I2CONCLR=1<<si;
	while(((I2CONSET>>si)&1)==0);
	return I2DAT;
}

unsigned char i2c_ack()
{
	I2CONSET=aa;
	I2CONCLR=1<<si;
	while(((I2CONSET>>si)&1)==0);
	I2CONCLR=aa;
	return I2DAT;
}

void delay(unsigned int ms)
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<ms);
	T0TCR=0X02;
	T0TCR=0X00;
}

void eeprom_write(unsigned char slave,unsigned char buff,unsigned char dat)
{
	i2c_start();
	i2c_write(slave<<1);
	i2c_write(buff);
	i2c_write(dat);
	i2c_stop();
	delay(10);
}

unsigned char eeprom_read(unsigned char slave,unsigned char buff)
{
	unsigned char	dat;
	i2c_start();
	i2c_write(slave<<1);
	i2c_write(buff);
	i2c_restart();
	i2c_write(slave<<1|1);
	dat=i2c_nack();
	i2c_stop();
	return dat;
}

void page_write(unsigned char slave,unsigned char buff,unsigned char *s,unsigned char nBytes)
{
	unsigned char i;
	i2c_start();
	i2c_write(slave<<1);
	i2c_write(buff);
	for(i=0;i<nBytes;i++)
	{
		i2c_write(s[i]);
	}
	i2c_stop();
	delay(10);
	
}

void seq_read(unsigned char slave,unsigned char buff,unsigned char *p,unsigned char nBytes)
{
	unsigned char i;
	i2c_start();
	i2c_write(slave<<1);
	i2c_write(buff);
	i2c_restart();
	i2c_write(slave<<1|1);
	for(i=0;i<nBytes-1;i++)
	{
		p[i]=i2c_ack();
	}
	p[i]=i2c_nack();
	i2c_stop();
	delay(10);
	
}

void dec_to_BCD(unsigned char *s)
{
	unsigned char i,a,b;
	for(i=0;i<7;i++)
	{
		a=s[i]%10;
		b=s[i]/10;
		s[i]=(b<<4) + a;
		
	}
}
void BCD_to_dec(unsigned char *s)
{
	unsigned char i,a,b;
	for(i=0;i<7;i++)
	{
		a=s[i]&0x0f;
		b=(s[i]>>4)&0x0f;
		s[i]=b*10 + a;
		
	}
}
unsigned char p[9] __attribute__((at(0x40000040)))="";

int main()
{
	//unsigned char ch;
	//char p="";
	unsigned char a[]={0,47,12,7,11,7,26};
	dec_to_BCD(a);
	IODIR0=led0|led1;
	i2c_init();
	//eeprom_write(0x50,0x00,'A');
	//ch=eeprom_read(0x50,0x00);

	page_write(0x68,0x00,a,7);
	seq_read(0x68,0x00,p,7);
	BCD_to_dec(p);
	delay(100);

}




















