#ifndef I2C_H
#define I2C_H

#define scl (1 << 4)
#define sda (1 << 6)
#define aa  (1 << 2)
#define si  3
#define sto (1 << 4)
#define sta (1 << 5)
#define i2en (1 << 6)
#define loadval 75

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

#endif
