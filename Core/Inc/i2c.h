#include "stdint.h"
#include "stdio.h"

void i2c1_scan_bus(void);
void i2c_init(void);
void i2c1_Write_Byte(uint8_t saddr, uint8_t data);
