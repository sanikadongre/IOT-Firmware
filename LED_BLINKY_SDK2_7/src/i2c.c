/**************************************************************************************************
 * The standard header files like em_i2c.h, em_gpio.h, sleep.h are being developed by SiliconLabs
 *  This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/
#include "cmu.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "sleep.h"
#include "letimer.h"
#include "gpio.h"
#include "sleep_timer.h"
#include "i2c.h"
CMU_ClkDiv_TypeDef prescaler = 1;
int temp_1;

void i2c_setup(void)
{
	GPIO_PinOutSet(SI7021_Port, SI7021_Pin);
	CMU_ClockEnable(cmuClock_HFPER,true);
	CMU_ClockEnable(cmuClock_I2C0, true);
	I2C0->ROUTEPEN = (I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN);
	I2C0->ROUTELOC0 = I2C_ROUTELOC0_SCLLOC_LOC14 | I2C_ROUTELOC0_SDALOC_LOC16;
	I2C_Init_TypeDef i2c_init_parameter = I2C_INIT_DEFAULT;
	I2C_Init(I2C0,&i2c_init_parameter);
	if (I2C0->STATE & I2C_STATE_BUSY)
			{
				I2C0->CMD = I2C_CMD_ABORT;
			}
	//GPIO_PinModeSet(SI7021_Port, SI7021_Pin,gpioModePushPull,1);
}

void Temp_Sensor_on(void)
{
	GPIO_PinModeSet(SI7021_Port, SI7021_Pin,gpioModePushPull,1);
}

void Temp_Sensor_off(void)
{
	GPIO_PinModeSet(I2C0_SCL_PORT, I2C0_SCL_PIN, gpioModeDisabled, 1);
	GPIO_PinModeSet(I2C0_SDA_PORT, I2C0_SDA_PIN, gpioModeDisabled, 1);
	GPIO_PinModeSet(SI7021_Port, SI7021_Pin,gpioModePushPull,0);
	unblockSleepMode(EM3);
	unblockSleepMode(EM2);
}

void temp_find_record(uint16_t temp_value)
{

	uint16_t temp;
	temp = ((17572*temp/65536)-(4685/100));
	if(temp < temp_threshold)
	{
		GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_ON);
	}
	else
	{
		GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_OFF);
	}
	blockSleepMode(EM2);
	uint16_t temp_data=0;

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
	temp_data = i2c_write_read(SLAVE_ADDRESS);
	temp_data = temp_data << 8;
	temp_data |= i2c_second();
	i2c_transfer_complete();
	temp_find_record(temp_data);

}

uint16_t i2c_write_read(uint8_t address)
{
	uint8_t data = 0xE3;
	address = SLAVE_ADDRESS;
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
	return I2C0->RXDATA;
}

uint8_t i2c_second(void)
{
	I2C0->CMD = I2C_CMD_ACK;
	//while((I2C0->IF & I2C_IF_RXDATAV ) == 0);
	return (I2C0->RXDATA);
}

void i2c_transfer_complete(void)
{
	I2C0->CMD = I2C_CMD_NACK;
	I2C0->CMD = I2C_CMD_STOP;
}
