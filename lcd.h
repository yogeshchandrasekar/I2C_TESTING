#include "DELAY.H"
#define LCD_D (0xff<<16)
#define EN (1<<24)
#define RS (1<<25)
void LCD_COMMAND(char cmd){
	IOCLR0=LCD_D;
	IOSET0=cmd<<4;
	IOCLR0=RS;
	IOSET0=EN;
	delay_ms(2);
	IOCLR0=EN;
	delay_ms(2);
}
void LCD_INIT(void){
	PINSEL1 &= ~(0x000FFFFF);
	IODIR0|=LCD_D|EN|RS;
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X02);
	LCD_COMMAND(0X0C);
	LCD_COMMAND(0X38);
}

void LCD_DATA(char d){
	IOCLR0=LCD_D;
	IOSET0=d<<4;
	IOSET0=RS;
	IOSET0=EN;
	delay_ms(2);
	IOCLR0=EN;
	delay_ms(2);
}
void LCD_STRING(char *str){
	while(*str){
		LCD_DATA(*str++);
	}
}
	
	
