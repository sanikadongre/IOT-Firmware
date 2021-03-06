/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/
/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "ble-configuration.h"
#include "board_features.h"
#include "main.h"
#include "gpio.h"
#include "cmu.h"
#include "em_letimer.h"
/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "em_chip.h"
#include "sleep_timer.h"
#include "i2c.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"
#include "sleep.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif


/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;
//***********************************************************************************
// Include files
//***********************************************************************************



//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************


//***********************************************************************************
// main
//***********************************************************************************

/**
 * @brief  Main function
 *
 */

int main(void)
{
	uint8_t prescaler= 1;

	CHIP_Init();
    uint32_t i=0;
 /* peripheral initialization routine();
  peripheral_call();*/
    // Initialize device
    initMcu();
    // Initialize board
    initBoard();

    /* Initialize GPIO */
    gpio_init();
    // Initialize clocks
    cmu_init();

    // Initialize stack
    gecko_init(&config);
    //block_sleep_mode(EM3);
    //block_sleep_mode(EM2);
    //block_sleep_mode(EM1);
    i2c_setup();
	cmu_letimer(prescaler);
    Temp_Sensor_on();
    uint16_t temp_value =0;
	temp_find_record(temp_value);
	Temp_Sensor_off();
	while(1)
	{
	    sleep();
	/*if (schedule_event == 0) sleep();
	if (schedule_event & event_1)
	{ code servicing event 1;
	schedule_event &= ~event_1;
	} if (schedule_event & event_2)
	{ code servicing event 2;
	schedule_event &= ~event_2;}
	}*/
	}
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
