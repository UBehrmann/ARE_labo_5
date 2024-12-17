/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : io_function.c
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

#include "io_function.h"

#include <stdio.h>
#include <stdbool.h>


static bool Keys_read(void)
{
    /* Keys are active low. Invert bits */
    int read_data = ~AXI_HPS_LABO_REG(ARE_PW5_BUTTONS_ADDR) & ARE_PW5_BUTTONS_MASK;

    /* Offset to align to LSb */
    read_data = read_data >> ARE_PW5_BUTTONS_OFFSET;

    return read_data ? true : false;
} /* Keys_read */


void Switchs_init(void)
{
    if(Switchs_read())
    {
        printf("%s", "Warning: some of the switches are in the ON position\n");
    } /* if */
}


void Leds_init(void)
{
    /* Reset all LED bits */
    Leds_write(0u);
}


void Keys_init(void)
{
    /* Keys are active low */
    if(Keys_read())
    {
        printf("%s", "Warning: some of the keys are being pressed\n");
    } /* if */
}


//***********************************//
//****** Global usage function ******//

uint32_t Switchs_read(void)
{
    int read_data = AXI_HPS_LABO_REG(ARE_PW5_SWITCHES_ADDR) & ARE_PW5_SWITCHES_MASK;
    return read_data >> ARE_PW5_SWITCHES_OFFSET;
} /* Switchs_read */


void Leds_write(uint32_t value)
{
    /* In this function, we either turn all LEDs ON or OFF */
    if(value == 0)
    {
    	AXI_HPS_LABO_REG(ARE_PW5_LEDS_ADDR) &= ~AXI_HPS_LABO_REG(ARE_PW5_LEDS_ADDR);
    }
    else
    {
        Leds_set(ARE_PW5_LEDS_MASK);
    } /* if */
} /* Leds_write */


void Leds_set(uint32_t maskleds)
{
    /* Only enable the ones in the mask */
	AXI_HPS_LABO_REG(ARE_PW5_LEDS_ADDR) |= (maskleds << ARE_PW5_LEDS_OFFSET) & ARE_PW5_LEDS_MASK;
} /* Leds_set */


void Leds_clear(uint32_t maskleds)
{
	AXI_HPS_LABO_REG(ARE_PW5_LEDS_ADDR) &= (~maskleds << ARE_PW5_LEDS_OFFSET) & ARE_PW5_LEDS_MASK;
} /* Leds_clear */


void Leds_toggle(uint32_t maskleds)
{
    /* Only change required LEDs */
	AXI_HPS_LABO_REG(ARE_PW5_LEDS_ADDR) ^= (maskleds << ARE_PW5_LEDS_OFFSET) & ARE_PW5_LEDS_MASK;
} /* Leds_toggle */


bool Key_read(int key_number)
{
    bool ret_val = false;

    /* Only perform check if requested key number is valid */
    if(key_number >= 0 && key_number < ARE_PW5_BUTTONS_NBR)
    {
        /* Keys are active low. Invert bits */
        int read_data = ~AXI_HPS_LABO_REG(ARE_PW5_BUTTONS_ADDR) & ARE_PW5_BUTTONS_MASK;

        /* Offset to align to LSb, and apply key mask */
        read_data = read_data >> ARE_PW5_BUTTONS_OFFSET;
        read_data &= 1 << key_number;

        ret_val = read_data ? true : false;
    } /* if */

    return ret_val;
} /* Key_read */
