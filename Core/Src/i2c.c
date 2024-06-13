#include "i2c.h"
#include "stm32f1xx.h"                  // Device header



void i2c_init() {
	/*Enable clock access to GPIOB*/
	RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;

	/*Set PB6 to output 50MHz*/
	GPIOB->CRL|=GPIO_CRL_MODE6;
	/*Set PB6 to ALternate Open drain*/
	GPIOB->CRL|=GPIO_CRL_CNF6;

	/*Set PB7 to output 50MHz*/
	GPIOB->CRL|=GPIO_CRL_MODE7;
	/*Set PB7 to ALternate Open drain*/
	GPIOB->CRL|=GPIO_CRL_CNF7;

	/*Enable clock access to alternate function of the pins*/
	RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;

	/*Enable clock access to I2C1*/
	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;


	/*Tell the peripheral that the clock is 8MHz*/
	I2C1->CR2&=~(I2C_CR2_FREQ);
	I2C1->CR2|=(8<<I2C_CR2_FREQ_Pos);
	/*Set the rise time*/
	I2C1->TRISE=9;

	I2C1->CCR|=0x28;

	I2C1->CR1|=I2C_CR1_PE;
}

void i2c1_Write_Byte(uint8_t saddr, uint8_t data)
{
	while(I2C1->SR2&I2C_SR2_BUSY){;}          /*wait until bus not busy*/
	I2C1->CR1|=I2C_CR1_START;                 /*generate start*/
	while(!(I2C1->SR1&I2C_SR1_SB)){;}         /*wait until start bit is set*/
	I2C1->DR = saddr<< 1;                 	 /* Send slave address with write operation*/
	while(!(I2C1->SR1&I2C_SR1_ADDR)){;}      /*wait until address flag is set*/
	(void)I2C1->SR2; 						/*clear SR2 by reading it */
	while(!(I2C1->SR1&I2C_SR1_TXE)){;}       /*Wait until Data register empty*/
	I2C1->DR = data;                        /* send memory address*/
	while (!(I2C1->SR1 & I2C_SR1_BTF));      /*wait until transfer finished*/
	I2C1->CR1 |=I2C_CR1_STOP;				/*Generate Stop*/
}

static void delay(uint32_t delay)
{
	for (volatile int i=0;i<1000;i++);
}

void i2c1_scan_bus(void)
{
	int a=0;
	for (uint8_t i=0;i<128;i++)
	{
		I2C1->CR1 |= I2C_CR1_START;
		while(!(I2C1->SR1 & I2C_SR1_SB));
		I2C1->DR=(i<<1|0);
		while(!(I2C1->SR1)|!(I2C1->SR2)){};
		I2C1->CR1 |= I2C_CR1_STOP;
		delay(100);//minium wait time is 40 uS, but for sure, leave it 100 uS
		a=(I2C1->SR1&I2C_SR1_ADDR);
		if (a==2)
		{
//			printf("Found I2C device at address 0x%X (hexadecimal), or %d (decimal)\r\n",i,i);
		}
	}
}
