#ifndef TEMPERATURE_I2C_H_
#define TEMPERATURE_I2C_H_
#include<stdint.h>
#include "cmu.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "sleep.h"
#include "letimer.h"
#include "gpio.h"


#define temp_threshold (15)
//#define I2C0_LOW_MODE(EM1)
#define SLAVE_ADDRESS	(0x40)
#define IO_EXPANDER_ADDRESS		(0x90)
#define TEMP_SENSOR (0xE3)
#define write_command	(0xFE)
#define read_command	(0x01)
#define TEMP_SENSOR_HEATER_OFF	(0x00)
#define SI7021_Port gpioPortD
#define SI7021_Pin 9
#define I2C0_SCL_PIN 	AF_I2C0_SCL_PIN(_I2C_ROUTELOC0_SCLLOC_LOC14)
#define I2C0_SDA_PIN 	AF_I2C0_SDA_PIN(_I2C_ROUTELOC0_SDALOC_LOC16)
#define I2C0_SCL_PORT	AF_I2C0_SCL_PORT(_I2C_ROUTELOC0_SCLLOC_LOC14)
#define I2C0_SDA_PORT 	AF_I2C0_SDA_PORT(_I2C_ROUTELOC0_SDALOC_LOC16)

void Temp_Sensor_on(void);
void Temp_Sensor_off(void);
void i2c_setup(void);
void temp_find_record(uint16_t temp_value);
uint16_t i2c_write_read(uint8_t address);
uint8_t i2c_second(void);
void i2c_transfer_complete(void);
#endif /* TEMPERATURE_I2C_H_ */
