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
 *
 * The emlib functions, the sleep routines and other functions are created by Silicon labs
 **************************************************************************************************/


#include "i2c.h"
#include "letimer.h"
#include "init_mcu.h"
#include "init_board.h"
#include "infrastructure.h"
#include "ble-configuration.h"
#include "board_features.h"
#include "main.h"
#include "gpio.h"
#include "cmu.h"
#include "em_system.h"
/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "em_chip.h"
#include "sleep_timer.h"
#include "em_core.h"
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
#include "gatt_db.h"
#include "i2cspm.h"
//#include "si7013.h"
//#include "tempsens.h"
//#include "si7013.h"
//#include "tempsens.h"
uint8_t mode;
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
uint8_t BLE_CON;
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

#define tx_max (80)
#define tx_min  (-260)
#define CONN_INTERVAL (75)
#define SLAVE_INTERVAL (450)
#define SLAVE_LATENCY ((SLAVE_INTERVAL/CONN_INTERVAL)-1)
#define CON_VALUE (CONN_INTERVAL/1.25)
#define advertising_parameter 539
#define TIME_OUT 0x1E0

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
void temperature_read()
   {
     uint8_t htmTempBuffer[5]; /* Stores the temperature data in the Health Thermometer (HTM) format. */
     uint8_t flags = 0x00;   /* HTM flags set as 0 for Celsius, no time stamp and no temperature type. */
     uint8_t *p = htmTempBuffer; /* Pointer to HTM temperature buffer needed for converting values to bitstream. */
    int temp1=0;
    Temp_sensor_on();
     while(event_1!=0x35);
     event_1=0;
     GPIO_PinModeSet(I2C0_SCL_PORT, I2C0_SCL_PIN, gpioModeWiredAnd, 1);
     GPIO_PinModeSet(I2C0_SDA_PORT, I2C0_SDA_PIN, gpioModeWiredAnd, 1);
     for (int i = 0; i < 9; i++)
     		{
     			GPIO_PinOutClear(I2C0_SCL_PORT, I2C0_SCL_PIN);
     			GPIO_PinOutSet(I2C0_SCL_PORT, I2C0_SCL_PIN);
     		}
     I2C0->IFC = 0x7FF;
     	I2C0->IEN |= (I2C_IEN_ACK | I2C_IEN_RXDATAV);
     	I2C_Enable(I2C0,true);
  UINT8_TO_BITSTREAM(p,flags);
  temp1 = Temp_record();
  uint32_t t= FLT_TO_UINT32(temp1,0);
  UINT32_TO_BITSTREAM(p, t);
  Temp_sensor_off();
    gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_temperature_measurement, 5, htmTempBuffer);

   }
int main(void)
{
	CHIP_Init();
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
    //blockSleepMode(EM3);
    //blockSleepMode(EM2);
    //blockSleepMode(EM1);
    i2c_setup();
    CMU_OscillatorEnable(cmuOsc_LFXO,true,true);
   CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
   CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
   CMU_ClockEnable(cmuClock_LETIMER0, true);
   CMU_ClockEnable(cmuClock_GPIO, true);
    LETIMER_setup();

	while(1)
	{
		/* Event pointer for handling events */
		    struct gecko_cmd_packet* evt;

		    /* Check for stack event. */
		    evt = gecko_wait_event();

		    /* Handle events */
		    switch (BGLIB_MSG_ID(evt->header)) {
		      /* This boot event is generated when the system boots up after reset.
		       * Do not call any stack commands before receiving the boot event.
		       * Here the system is set to start advertising immediately after boot procedure. */
		      case gecko_evt_system_boot_id:

		        /* Set advertising parameters. 100ms advertisement interval. All channels used.
		         * The first two parameters are minimum and maximum advertising interval, both in
		         * units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */
		        gecko_cmd_le_gap_set_adv_parameters(advertising_parameter, advertising_parameter, 7);
                //gecko_cmd_le_gap_set_conn_parameters(60,60,6,60);
               // gecko_cmd_le_connection_set_parameters(0x01,60,6,60);
                //gecko_cmd_system_set_tx_power(0);
		        /* Start general advertising and enable connections. */
		        gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
		        break;

		      case gecko_evt_le_connection_closed_id:

		        /* Check if need to boot to dfu mode */
		        if (boot_to_dfu) {
		          /* Enter to DFU OTA mode */
		          gecko_cmd_system_reset(2);
		        } else {
		          /* Restart advertising after client has disconnected */
		          gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
		        }
		        break;
		      case gecko_evt_le_connection_opened_id:
		    	  BLE_CON = evt->data.evt_le_connection_opened.connection;
		    	  gecko_cmd_le_connection_set_parameters(BLE_CON,CON_VALUE, CON_VALUE, SLAVE_LATENCY, TIME_OUT);
               break;
		      case gecko_evt_system_external_signal_id:
		      {
		    	  if(mode==1)
		    	  {
		    		temperature_read();
		    		Temp_sensor_off();

		    	  mode=0;
		    	  break;
		           }
		    	  else
		    	  {
		    		  Temp_sensor_on();
		    		  mode=1;
		    		  break;
		    	  }
		    	  }
		         break;
		      case gecko_evt_gatt_server_characteristic_status_id:
		    	   gecko_cmd_le_connection_get_rssi(0x01);
		    	  break;

		      case gecko_evt_le_connection_rssi_id:
		    	  gecko_cmd_system_halt(1);
		    	  if(evt->data.evt_le_connection_rssi.rssi > -35){
		    		  gecko_cmd_system_set_tx_power(tx_min);
		    	  }
		    	  else if((evt->data.evt_le_connection_rssi.rssi > -45)&&(evt->data.evt_le_connection_rssi.rssi <-35))
		    	  {
		    		  gecko_cmd_system_set_tx_power(-200);
		    	  }
		    	  else if((evt->data.evt_le_connection_rssi.rssi > -55)&&(evt->data.evt_le_connection_rssi.rssi <-45))
		    	 		    	  {
		    	 		    		  gecko_cmd_system_set_tx_power(-150);
		    	 		    	  }
		    	  else if((evt->data.evt_le_connection_rssi.rssi > -65)&&(evt->data.evt_le_connection_rssi.rssi <-55))
		    	  		    	 		    	  {
		    	  		    	 		    		  gecko_cmd_system_set_tx_power(-50);
		    	  		    	 		    	  }
		    	  else if((evt->data.evt_le_connection_rssi.rssi > -75)&&(evt->data.evt_le_connection_rssi.rssi <-65))
		    	  		    {
		    	  		       gecko_cmd_system_set_tx_power(0);
		    	  		    	   }
		    	  else if((evt->data.evt_le_connection_rssi.rssi > -85)&&(evt->data.evt_le_connection_rssi.rssi <-75))
		    	 		    {
		    	 		    	 gecko_cmd_system_set_tx_power(50);


		    }

		    	 else {
		    		  gecko_cmd_system_set_tx_power(tx_max);
		    	}
		    	  gecko_cmd_system_halt(0);
		    	  break;
		      /* Events related to OTA upgrading
		         ----------------------------------------------------------------------------- */

		      /* Check if the user-type OTA Control Characteristic was written.
		       * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
		      case gecko_evt_gatt_server_user_write_request_id:

		        if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
		          /* Set flag to enter to OTA mode */
		          boot_to_dfu = 1;
		          /* Send response to Write Request */
		          gecko_cmd_gatt_server_send_user_write_response(
		            evt->data.evt_gatt_server_user_write_request.connection,
		            gattdb_ota_control,
		            bg_err_success);
		          GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED_ON);

		          /* Close connection to enter to DFU OTA mode */
		          gecko_cmd_endpoint_close(evt->data.evt_gatt_server_user_write_request.connection);
		        }
		        break;

		      default:
		        break;
		    }
		  }
		//sleep();
	/*if(event_1==1)
		{
			Temp_sensor_on();
			event_1=0;
		}
		else if(event_2==1)
		{
		     uint16_t k;
			 k = Temp_record();
			Temp_sensor_off();
			event_2=0;
		}
	}*/
}


/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
