/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : io_function.h
 * Author               : Pedro Alves da Silva
 * Date                 : 29 november 2024
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: I/O functions
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student           Comments
 * 1.0    6 oct 24    pedro.alvesdas    First version
*****************************************************************************************/

#ifndef SRC_IO_IO_FUNCTION_H_
#define SRC_IO_IO_FUNCTION_H_

#include "axi_lw.h"
#include <stdint.h>
#include <stdbool.h>


/***************************
 * I/O buttons (RO)
 ***************************/

#define ARE_PW5_BUTTONS_ADDR		(0x04)
#define ARE_PW5_BUTTONS_OFFSET		(0x00)
#define ARE_PW5_BUTTONS_MASK		(0x00000007)
#define ARE_PW5_BUTTONS_NBR        	(3u)


/***************************
 * I/O switches (RO)
 ***************************/

#define ARE_PW5_SWITCHES_ADDR		(0x08)
#define ARE_PW5_SWITCHES_OFFSET		(0x00)
#define ARE_PW5_SWITCHES_MASK		(0x000003FF << ARE_PW5_SWITCHES_OFFSET)
#define ARE_PW5_SWITCHES_NBR        (10u)


/***************************
 * I/O LEDs (RO)
 ***************************/

#define ARE_PW5_LEDS_ADDR			(0x0C)
#define ARE_PW5_LEDS_OFFSET			(0x00)
#define ARE_PW5_LEDS_MASK			(0x000003FF << ARE_PW5_LEDS_OFFSET)
#define ARE_PW5_LEDS_NBR        	(10u)


/***************************
 * Interface status (RO)
 ***************************/

#define ARE_PW5_STATUS_ADDR			(0x10)
#define ARE_PW5_STATUS_OFFSET		(0x00)
#define ARE_PW5_STATUS_MASK			(0x00000003 << ARE_PW5_STATUS_OFFSET)


/***************************
 * Modes & delays (RW)
 ***************************/

#define ARE_PW5_DELAY_ADDR			(0x14)
#define ARE_PW5_DELAY_OFFSET		(0x00)
#define ARE_PW5_DELAY_MASK			(0x00000003 << ARE_PW5_DELAY_OFFSET)
#define ARE_PW5_DELAY_1HZ			(0b00 << ARE_PW5_DELAY_OFFSET)
#define ARE_PW5_DELAY_1KHZ			(0b01 << ARE_PW5_DELAY_OFFSET)
#define ARE_PW5_DELAY_100KHZ		(0b10 << ARE_PW5_DELAY_OFFSET)
#define ARE_PW5_DELAY_1MHZ			(0b11 << ARE_PW5_DELAY_OFFSET)

#define ARE_PW5_MODE_ADDR			ARE_PW5_DELAY_ADDR
#define ARE_PW5_MODE_OFFSET			(0x04)
#define ARE_PW5_MODE_MASK			(0x00000001 << ARE_PW5_MODE_OFFSET)
#define ARE_PW5_MODE_MANUAL			(0b0 << ARE_PW5_MODE_OFFSET)
#define ARE_PW5_MODE_AUTO			(0b1 << ARE_PW5_MODE_OFFSET)


/***************************
 * Controls (WO)
 ***************************/

#define ARE_PW5_INIT_CHAR_ADDR		ARE_PW5_STATUS_ADDR
#define ARE_PW5_INIT_CHAR_MASK		(1u)
#define ARE_PW5_NEW_CHAR_ADDR		ARE_PW5_STATUS_ADDR
#define ARE_PW5_NEW_CHAR_MASK		(1u << 4u)


/***************************
 * Characeters (RO)
 ***************************/

#define ARE_PW5_CHAR_0_OFFSET		(0x00u)
#define ARE_PW5_CHAR_1_OFFSET		(0x01u)
#define ARE_PW5_CHAR_2_OFFSET		(0x02u)
#define ARE_PW5_CHAR_3_OFFSET		(0x03u)

#define ARE_PW5_CHARS_0_ADDR		(0x20)
#define ARE_PW5_CHARS_1_ADDR		(0x24)
#define ARE_PW5_CHARS_2_ADDR		(0x28)
#define ARE_PW5_CHARS_3_ADDR		(0x2C)

#define ARE_PW5_CHECKSUM_ADDR		(0x30)
#define ARE_PW5_CHECKSUM_OFFSET		(0x00)
#define ARE_PW5_CHECKSUM_MASK		(0xFF << ARE_PW5_CHECKSUM_OFFSET)


/***************************
 * Functions
 ***************************/

// Swicths_init function : Initialize all Switchs in PIO core (SW9 to SW0)
void Switchs_init(void);

// Leds_init function : Initialize all Leds in PIO core (LED9 to LED0)
void Leds_init(void);

// Keys_init function : Initialize all Keys in PIO core (KEY3 to KEY0)
void Keys_init(void);

// Switchs_read function : Read the switchs value
// Parameter : None
// Return : Value of all Switchs (SW9 to SW0)
uint32_t Switchs_read(void);

// Leds_write function : Write a value to all Leds (LED9 to LED0)
// Parameter : "value"= data to be applied to all Leds
// Return : None
void Leds_write(uint32_t value);

// Leds_set function : Set to ON some or all Leds (LED9 to LED0)
// Parameter : "maskleds"= Leds selected to apply a set (maximum 0x3FF)
// Return : None
void Leds_set(uint32_t maskleds);

// Leds_clear function : Clear to OFF some or all Leds (LED9 to LED0)
// Parameter : "maskleds"= Leds selected to apply a clear (maximum 0x3FF)
// Return : None
void Leds_clear(uint32_t maskleds);

// Leds_toggle function : Toggle the curent value of some or all Leds (LED9 to LED0)
// Parameter : "maskleds"= Leds selected to apply a toggle (maximum 0x3FF)
// Return : None
void Leds_toggle(uint32_t maskleds);

// Key_read function : Read one Key status, pressed or not (KEY0 or KEY1 or KEY2 or KEY3)
// Parameter : "key_number"= select the key number to read, from 0 to 3
// Return : True(1) if key is pressed, and False(0) if key is not pressed
bool Key_read(int key_number);

#endif /* SRC_IO_IO_FUNCTION_H_ */
