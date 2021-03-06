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


/***** Credit for the Sleep functions and Letimer functions goes to Silicon Labs *****/
/***** Note: All the functions defined in the following files used in my code were developed by Silicon Labs ->
sleep.h, em_letimer.h, em_emu.h, em_core.h, em_cmu.h, em_assert.h, graphics.h, native_gecko.h, bg_types.h,
I have referred the code for accelerometer, mitm, encryption-decryption from silicon labs
I have referred the client  and server spp sample examples from silicon labs website to develop my project.
I have used the event scheduler developed by silicon labs.
*****/


/* Board headers */

//***********************************************************************************
// defined files
//***********************************************************************************
#define reset_counter_address (0x4004)
#define gattdb_temp_measurement (15)
#ifndef MAX_CONNECTIONSinlcude
#define MAX_CONNECTIONS 4
#endif
#define Max_Transmit_Power	(80)
#define Min_Transmit_Power	(-260)
#define Slave_interval	(450)
#define Latency	((Slave_interval/(1.25*Connection_interval))-1)
#define Connection_interval	(75/1.25)
#define Advertise (529)
#define Timeout (480)
#include "init_mcu.h"
#include "init_board.h"
#include "ble-configuration.h"
#include "board_features.h"
#include "gpio.h"
#include "cmu.h"
#include "em_letimer.h"
#include "persistent_memory.h"
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "em_chip.h"
#include <stdint.h>
/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "graphics.h"
#include "main.h"
#include "sleep.h"
#include <stdio.h>
#include "black_box.h"
#include "accelerometer.h"

//#include  "si7013.h"
/* Device initialization header */
#include "hal-config.h"
#include "sleep.h"
#include "infrastructure.h"
#include "letimer.h"
//#include "main.h"
#include "gatt_db.h"
#ifdef HAL_CONFIG
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

uint8_t BLE_CON;
uint8_t activeConnectionId =1;
bool read_pk, read_y_n;
//***********************************************************************************
// global variables
//***********************************************************************************
#define DISCONNECTED	0
#define SCANNING		1
#define FIND_SERVICE	2
#define FIND_CHAR		3
#define ENABLE_NOTIF 	4
#define DATA_MODE		5
// SPP service UUID: eec63950-03ce-4fbf-a8e9-757d254c3348
const uint8 next_move_service_UUID[16] = {0x48, 0x33, 0x4c, 0x25, 0x7d, 0x75, 0xe9, 0xa8, 0xbf, 0x4f, 0xce, 0x03, 0x50, 0x39, 0xc6, 0xee};


// SPP data UUID: = 2a4634a3-6880-40ae-b72d-e71b9e451296
const uint8 next_move_char_UUID[16] = {0x96, 0x12, 0x45, 0x9e, 0x1b, 0xe7, 0x2d, 0xb7, 0xae, 0x40, 0x80, 0x68, 0xa3, 0x34, 0x46, 0x2a};

//SPP service UUID: 516ddb5a-7b9b-49f1-9aa5-1e2670bd1b51
const uint8 Button_Service_UUID[16] = {0x51, 0x1b, 0xbd, 0x70, 0x26, 0x1e, 0xa5, 0x9a, 0xf1, 0x49, 0x9b, 0x7b, 0x5a, 0xdb, 0x6d, 0x51};

//SPP data UUID: ffeeed0a-9e66-4e74-a1f9-1bc31164d42c
const uint8 Button_Char_UUID[16] = {0x2c, 0xd4, 0x64, 0x11, 0xc3, 0x1b, 0xf9, 0xa1, 0x74, 0x4e, 0x66, 0x9e, 0x0a, 0xed, 0xee, 0xff};

#define RESTART_TIMER 1
#define SPP_TX_TIMER  2
static uint8 _conn_handle = 0xFF;
static int _main_state;
static uint32 _service_handle_light;
static uint32 _service_handle_move;
static uint16 _char_handle_light;
static uint16 _char_handle_move;
uint8_t mode = 0;

static void reset_variables()
{
	_conn_handle = 0xFF;
	_main_state = DISCONNECTED;
	_service_handle_light = 0;
	_service_handle_move = 0;
	_char_handle_light = 0;
	_char_handle_move = 0;
}

uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

uint8_t boot_to_dfu = 0;


static const gecko_configuration_t config =
{
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

static int process_scan_response(struct gecko_msg_le_gap_scan_response_evt_t *pResp)
{
	// decoding advertising packets is done here. The list of AD types can be found
	// at: https://www.bluetooth.com/specifications/assigned-numbers/Generic-Access-Profile

    int i = 0;
    int ad_match_found = 0;
	int ad_len;
    int ad_type;
    char name[32];
    while (i < (pResp->data.len - 1))
    {
		ad_len  = pResp->data.data[i];
		ad_type = pResp->data.data[i+1];
		if (ad_type == 0x08 || ad_type == 0x09 )
		{
			// type 0x08 = Shortened Local Name
			// type 0x09 = Complete Local Name
			memcpy(name, &(pResp->data.data[i+2]), ad_len-1);
			name[ad_len-1] = 0;
		}
		if (ad_type == 0x06 || ad_type == 0x07)
		{
			// type 0x06 = Incomplete List of 128-bit Service Class UUIDs
			// type 0x07 = Complete List of 128-bit Service Class UUIDs
			if((memcmp(Button_Service_UUID, &(pResp->data.data[i+2]),16) == 0)||((memcmp(next_move_service_UUID, &(pResp->data.data[i+2]),16) == 0)))
			{
				ad_match_found = 1;
			}
		}
		 i = i + ad_len + 1;
	}
	return(ad_match_found);
}

void main(void)
{
	uint32_t passkey = 0;
	uint8_t test_value = 8 , led_status = led_1_on, led_0_status = 0, led_1_status = 0, next_move=stop, command_signal=stop, prev_move=stop, external_event_counter = 0;
	uint8_t printbuf[128];
	uint8_t t=8;
    uint8array reset_counter;
    uint32_t random_number = 0;
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
	//GRAPHICS_Init();
	spi_setup();
	accelerometer_on();
    persistent_memory_load(&reset_counter,reset_counter_address );
    reset_counter.data[0]++;
    GRAPHICS_Clear();
	GRAPHICS_AppendString("Monish Sanika Project");
	GRAPHICS_AppendString("Reset Counter");
	print_number(reset_counter.data[0]);
    persistent_memory_save(&reset_counter,reset_counter_address);
	//black_box_init();
	GRAPHICS_Update();
	mode = 0;
	while(1)
	{
		/* Event pointer for handling events */
		struct gecko_cmd_packet* evt;

		/* Check for stack event. */
		evt = gecko_wait_event();

		/* Handle events */
		switch (BGLIB_MSG_ID(evt->header))
		{
			/* This boot event is generated when the system boots up after reset.
			* Do not call any stack commands before receiving the boot event.
			* Here the system is set to start advertising immediately after boot procedure. */
			case gecko_evt_system_boot_id:
			{
				/* Start general advertising and enable connections. */
				reset_variables();
				gecko_cmd_sm_delete_bondings();
				gecko_cmd_sm_configure(0x0F, sm_io_capability_displayyesno); /* Numeric comparison */
				gecko_cmd_sm_set_bondable_mode(1);
			    gecko_cmd_le_gap_discover(le_gap_discover_generic);
			    break;
			}

			case gecko_evt_le_gap_scan_response_id:
			{
				// process scan responses: this function returns 1 if we found the service we are looking for
				if(process_scan_response(&(evt->data.evt_le_gap_scan_response)) > 0)
				{
					struct gecko_msg_le_gap_open_rsp_t *pResp;
					// match found -> stop discovery and try to connect
					gecko_cmd_le_gap_end_procedure();
					pResp = gecko_cmd_le_gap_open(evt->data.evt_le_gap_scan_response.address, evt->data.evt_le_gap_scan_response.address_type);
					// make copy of connection handle for later use (for example, to cancel the connection attempt)
					_conn_handle = pResp->connection;
				}
				break;
			}

			case gecko_evt_le_connection_opened_id:
			{
				//	 start service discovery (we are only interested in one UUID)
				gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16, next_move_service_UUID);
				//gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16,next_move_service_UUID);
				_main_state = FIND_SERVICE;
				activeConnectionId = evt->data.evt_le_connection_opened.connection;
				gecko_cmd_sm_increase_security(activeConnectionId);
				break;
			}

			case gecko_evt_sm_confirm_passkey_id:
			{
				passkey = evt->data.evt_sm_confirm_passkey.passkey;
				GRAPHICS_Clear();
				GRAPHICS_AppendString("Passkey");
				print_number(passkey);
				GRAPHICS_Update();
				while(GPIO_PinInGet(PB0_port,PB0_pin)&&GPIO_PinInGet(PB1_port,PB1_pin));
				//black_box_init();
				letimer_init();
				break;
			}

			case gecko_evt_le_connection_closed_id:
			{
				reset_variables();
				SLEEP_SleepBlockEnd(sleepEM2); // enable sleeping after disconnect
				gecko_cmd_le_gap_discover(le_gap_discover_generic);
				break;
			}

			/*
			case gecko_evt_le_connection_parameters_id:
			{
				evt->data.evt_le_connection_parameters.interval;
				evt->data.evt_le_connection_parameters.txsize;
				break;
			}

			case gecko_evt_gatt_server_characteristic_status_id:
			{
				gecko_cmd_le_connection_get_rssi(0x01);
				break;
			}*/

			case gecko_evt_gatt_service_id:
			{
			    	if(evt->data.evt_gatt_service.uuid.len == 16)
			    	{
			    		if(memcmp(Button_Service_UUID, evt->data.evt_gatt_service.uuid.data,16) == 0)
						{
							_service_handle_light = evt->data.evt_gatt_service.service;
						}
						else if(memcmp(next_move_service_UUID, evt->data.evt_gatt_service.uuid.data,16) == 0)
						{
							_service_handle_move = evt->data.evt_gatt_service.service;
						}
			    	}
			    	break;
			}

			case gecko_evt_system_external_signal_id:
			{
				next_move = acceleration_calculate_value();
				if(GPIO_PinInGet(PB0_port,PB0_pin)||GPIO_PinInGet(PB1_port,PB1_pin))
				{
					if(!(GPIO_PinInGet(PB0_port,PB0_pin)))
					{
						led_status = led_0_on;
					}
					else if(!(GPIO_PinInGet(PB1_port,PB1_pin)))
					{
						led_status = led_1_on;
					}
					else
					{
						led_status = none_on;
					}
				}
				else
				{
					led_status = both_on;
				}
				break;
			}

			case gecko_evt_gatt_procedure_completed_id:
			{
		    	switch(_main_state)
		    	{
		    		case FIND_SERVICE:
		    		{
		    			/*if (_service_handle_light > 0)
						{
							// Service found, next search for characteristics
							gecko_cmd_gatt_discover_characteristics(_conn_handle, _service_handle_light);
							_main_state = FIND_CHAR;
						}*/
						if (_service_handle_move > 0)
						{
							// Service found, next search for characteristics
							gecko_cmd_gatt_discover_characteristics(_conn_handle, _service_handle_move);
							_main_state = FIND_CHAR;
						}
						break;
		    		}

		    		case FIND_CHAR:
		    		{
		    			/*if(_char_handle_light > 0)
						{
							gecko_cmd_gatt_write_characteristic_value_without_response(_conn_handle, _char_handle_move, 1, &test_value);
							gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16, next_move_service_UUID);
							//gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16,Button_Service_UUID);
						}*/
						if(_char_handle_move > 0)
						{
							gecko_cmd_gatt_write_characteristic_value_without_response(_conn_handle, _char_handle_move, 1, &next_move);
							gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16, next_move_service_UUID);
							//gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16,Button_Service_UUID);
						}
						_main_state = FIND_SERVICE;
		    		}
					default:
					{
						break;
					}
		    	}
				break;
			}

			case gecko_evt_gatt_characteristic_id:
			{
				if(evt->data.evt_gatt_characteristic.uuid.len == 16)
				{
					/*if(memcmp(Button_Char_UUID, evt->data.evt_gatt_characteristic.uuid.data,16) == 0)
					{
						_char_handle_light = evt->data.evt_gatt_characteristic.characteristic;
					}*/
					if(memcmp(next_move_char_UUID, evt->data.evt_gatt_characteristic.uuid.data,16) == 0)
					{
						_char_handle_move = evt->data.evt_gatt_characteristic.characteristic;
					}
				}
				break;
			}

			case gecko_evt_gatt_characteristic_value_id:
			{
				break;
			}

			default:
			{
				break;
			}
		}
	}
}
