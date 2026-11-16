#include "lcd.h"
#include "I2C.H"
#include "EEPROM.H"
int main(){
unsigned char p[20];
	unsigned int i;
	LCD_INIT();
 I2C_INIT();
	 LCD_COMMAND(0X80);
	 LCD_COMMAND(0X01);
	 LCD_STRING("I2C TESTING");
	 delay_ms(2000);
	 i2c_page_write(0x50,0x00,"WORKING",7);
	 i2c_eeprom_seq_read(0x50,0x00,p,7);
	 LCD_COMMAND(0X01);
	 LCD_COMMAND(0X80);
	 for(i=0;i<7;i++){
		 LCD_DATA(p[i]);
	 }
}

