#include "main.h"
#include "i2c.h"
#include "i2c_lcd.h"


int main(void)
{
	HAL_Init();
	i2c_init();
	i2c1_scan_bus();
	lcd_init();
	HAL_Delay(100);
	lcd_clear_display();
	lcd_goto_XY(1, 5);
	HAL_Delay(100);
	lcd_send_string("Trang thai");
	lcd_goto_XY(2, 0);
	HAL_Delay(100);
	lcd_send_string("Binh thuong");
	
}
