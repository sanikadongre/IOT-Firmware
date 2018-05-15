 /*
 * i2c.c
 * Author: Sanika
 */
/***** IP Credit: The emlib functions were developed by Silicon Labs *****/
#include "i2c.h"
#include "cmu.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "sleep.h"
#include "sleep_timer.h"
#include "gpio.h"
#include "letimer.h"
#include "em_letimer.h"

void i2c_setup(void)
{
	//GPIO_PinOutSet(SI7021_Port, SI7021_Pin);
	cmu_i2c(cmuClkDiv_1);
	I2C0->ROUTEPEN = (I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN);
	I2C0->ROUTELOC0 = I2C_ROUTELOC0_SCLLOC_LOC14 | I2C_ROUTELOC0_SDALOC_LOC16;
	I2C_Init_TypeDef i2c_init_parameter = I2C_INIT_DEFAULT;
	I2C_Init(I2C0,&i2c_init_parameter);
	if (I2C0->STATE & I2C_STATE_BUSY)
		{
			I2C0->CMD = I2C_CMD_ABORT;
		}
}
void Temp_sensor_on(void)
{
	GPIO_PinModeSet(SI7021_Port, SI7021_Pin,gpioModePushPull,1);
}

uint16_t Temp_record(void)
{
	//blockSleepMode(EM3);
	//blockSleepMode(EM2);
	GPIO_PinModeSet(I2C0_SCL_PORT, I2C0_SCL_PIN, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(I2C0_SDA_PORT, I2C0_SDA_PIN, gpioModeWiredAnd, 1);
	if (I2C0->STATE & I2C_STATE_BUSY)
		{
			I2C0->CMD = I2C_CMD_ABORT;
		}
	for (int i = 0; i < 9; i++)
		{
			GPIO_PinOutClear(I2C0_SCL_PORT, I2C0_SCL_PIN);
			GPIO_PinOutSet(I2C0_SCL_PORT, I2C0_SCL_PIN);
		}
	I2C_IntClear(I2C0, (I2C_IFC_START | I2C_IFC_RSTART | I2C_IFC_ADDR | I2C_IFC_TXC | I2C_IFC_ACK | I2C_IFC_NACK | I2C_IFC_MSTOP | I2C_IFC_ARBLOST | I2C_IFC_BUSERR | I2C_IFC_BUSHOLD |I2C_IFC_TXOF | I2C_IFC_RXUF | I2C_IFC_BITO | I2C_IFC_CLTO | I2C_IFC_SSTOP | I2C_IFC_RXFULL | I2C_IFC_CLERR));
	I2C_IntDisable(I2C0, (I2C_IEN_START | I2C_IEN_RSTART | I2C_IEN_ADDR | I2C_IEN_TXC | I2C_IEN_ACK | I2C_IEN_NACK | I2C_IEN_MSTOP | I2C_IEN_ARBLOST | I2C_IEN_BUSERR | I2C_IEN_BUSHOLD |I2C_IEN_TXOF | I2C_IEN_RXUF | I2C_IEN_BITO | I2C_IEN_CLTO | I2C_IEN_SSTOP | I2C_IEN_RXFULL | I2C_IEN_CLERR));
	I2C0->IFC = 0x7FF;
	I2C0->IEN |= (I2C_IEN_ACK | I2C_IEN_RXDATAV);
	I2C_Enable(I2C0,true);
	uint16_t temp_data=0;
	temp_data = i2c_write_read(SLAVE_ADDRESS,TEMP_DATA_READING);
	temp_data = ((temp_data/373) - 46.85);
	if(temp_data < temp_threshold)
	{
		GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED_ON);
	}
	else
	{
		GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED_OFF);
	}
	return temp_data;
}

uint16_t i2c_write_read(uint8_t address,uint8_t data)
{
	uint16_t temp_data=0;
	I2C0->TXDATA = (address << 1) & write_command;
	I2C0->CMD = I2C_CMD_START;
	while((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->TXDATA = data;
	while((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (address << 1) | read_command;
	while((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;
	while((I2C0->IF & I2C_IF_RXDATAV ) == 0);
	temp_data = I2C0->RXDATA;
	temp_data = temp_data << 8;
	I2C0->CMD = I2C_CMD_ACK;
	while((I2C0->IF & I2C_IF_RXDATAV ) == 0);
	temp_data |= I2C0->RXDATA;
	I2C0->CMD = I2C_CMD_NACK;
	I2C0->CMD = I2C_CMD_STOP;
	return temp_data;
}

void Temp_sensor_off(void)
{

	           GPIO_PinModeSet(I2C0_SCL_PORT, I2C0_SCL_PIN, gpioModeDisabled, 1);
			   GPIO_PinModeSet(I2C0_SDA_PORT, I2C0_SDA_PIN, gpioModeDisabled, 1);
			   GPIO_PinModeSet(SI7021_Port, SI7021_Pin,gpioModePushPull,0);
}
