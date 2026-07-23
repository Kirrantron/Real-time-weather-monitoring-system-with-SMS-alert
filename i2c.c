#include <LPC21xx.h>
#include "i2c.h"
#include "delay.h"



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
    I2CONCLR = (1 << si);
    while(((I2CONSET >> si) & 1) == 0);
    I2CONCLR = sta;
}

void i2c_write(unsigned char dat)
{
    I2DAT = dat;
    I2CONCLR = (1 << si);
    while(((I2CONSET >> si) & 1) == 0);
}

void i2c_stop(void)
{
    I2CONSET = sto;
    I2CONCLR = (1 << si);
}

unsigned char i2c_nack(void)
{
    I2CONSET = 0x00;
    I2CONCLR = (1 << si);
    while(((I2CONSET >> si) & 1) == 0);
    return I2DAT;
}

unsigned char i2c_ack(void)
{
    I2CONSET = aa;
    I2CONCLR = (1 << si);
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
        a = s[i] & 0x0F;
        b = (s[i] >> 4) & 0x0F;
        s[i] = (b * 10) + a;
    }
}
