#include "i2c.h"

void i2c_eeprom_write(unsigned char slaveAddr,unsigned char wBuffAddr,unsigned char dat)
{
	I2C_START();
	I2C_WRITE(slaveAddr<<1);
	I2C_WRITE(wBuffAddr);
	I2C_WRITE(dat);
	I2C_STOP();
	delay_ms(10);
}
unsigned char i2c_eeprom_read(unsigned char slaveAddr,unsigned char RBuffAddr)
{
	unsigned char dat=0;
	I2C_START();
	I2C_WRITE(slaveAddr<<1);
	I2C_WRITE(RBuffAddr);
	I2C_RESTART();
	I2C_WRITE(slaveAddr<<1|1);
	dat=I2C_NACK();
	I2C_STOP();
	delay_ms(10);
	return dat;
}
void i2c_page_write(unsigned char slaveAddr,unsigned char wBuffAddr,unsigned char *p,unsigned int n)
{
	unsigned int i=0;
	I2C_START();
	I2C_WRITE(slaveAddr<<1);
	I2C_WRITE(wBuffAddr);
	for(i=0;i<n;i++){
	I2C_WRITE(p[i]);
	}
	I2C_STOP();
	delay_ms(10);
}
void i2c_eeprom_seq_read(unsigned char slaveAddr,unsigned char rBuffStartAddr,unsigned char *p,unsigned char nBytes)
{
	unsigned int i;
	I2C_START();	
  I2C_WRITE(slaveAddr<<1);
	I2C_WRITE(rBuffStartAddr);    
  I2C_RESTART(); 	
	I2C_WRITE(slaveAddr<<1|1); 
	for(i=0;i<nBytes-1;i++)
	{
    p[i]=i2c_masterack();	
	}
	p[i]=I2C_NACK();
	I2C_STOP();
}
