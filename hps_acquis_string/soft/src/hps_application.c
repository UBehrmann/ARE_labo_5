/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : hps_application.c
 * Author               : 
 * Date                 : 
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Conception d'une interface évoluée sur le bus Avalon avec la carte DE1-SoC
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 
 *
*****************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "axi_lw.h"
#include "io/io_function.h"
#include "gen/gen_function.h"

int __auto_semihosting;



/***************************
 * Typedefs & structures
 ***************************/

/**
 * Function pointer called when a key is pressed
 */
typedef void(*TKeyPressHandler)();


/**
 * Structure containing both the action handler, and whether the key is pressed or not
 */
typedef struct
{
    /**
     * Handler when key is pressed
     * Does not fire again unless key has been released in the meantime
     */
    const TKeyPressHandler on_press;

    /**
     * Used to know whether the key has been released in the meantime or not
     */
    bool key_pressed;

    /**
     * Used to know whether continuous pressing is allowed
     */
    bool allows_continuous_pressing;
} TKeyPressAction;




/***************************
 * Global variables
 ***************************/

static int trustworthy_mode = 0;
static int error_count = 0;


/***************************
 * Key pressed handlers
 ***************************/

void key0_press_handler(void)
{
	/* Init chars */
	AXI_HPS_LABO_REG(ARE_PW5_INIT_CHAR_ADDR) = ARE_PW5_INIT_CHAR_MASK;
} /* key0_press_handler */


void key1_press_handler(void)
{
	/* Only perform action when in manual mode */
	uint32_t mode = (AXI_HPS_LABO_REG(ARE_PW5_MODE_ADDR) & ARE_PW5_MODE_MASK) >> ARE_PW5_MODE_OFFSET;
	if(ARE_PW5_MODE_MANUAL == mode)
	{
		AXI_HPS_LABO_REG(ARE_PW5_NEW_CHAR_ADDR) = ARE_PW5_NEW_CHAR_MASK;
	} /* if */
} /* key0_press_handler */


void key2_press_handler(void)
{
    char checksum_char = 0;
    char characters[ARE_PW5_CHARS_NUMBER] = {0};

    are_pw5_gen_read(characters, &checksum_char, trustworthy_mode);

    // Verify checksum : (char_1 + char_2 + ... + char_16 + checksum) modulo 256 = 0
    int calcul_integrity = 0;
    for(size_t i = 0; i < ARE_PW5_CHARS_NUMBER; ++i)
    {
    	calcul_integrity += characters[i];
    } /* if */
    calcul_integrity += checksum_char;
    calcul_integrity %= 256;
    bool checksum_ok = calcul_integrity == 0;

    int status = AXI_HPS_LABO_REG(ARE_PW5_STATUS_ADDR);

    if(checksum_ok){
        // OK : status: X , checksum: X, calcul integrity: X, string: X
        printf("OK : status: %d , checksum: %d, calcul integrity: %d, string: %s\n", status, checksum_char, calcul_integrity, characters);
    }else{
        // ER : status: X , checksum: X, calcul integrity: X, string: X 
        // ER : nombre d’erreur cumulée : X
        ++error_count;
        printf("ER : status: %d , checksum: %d, calcul integrity: %d, string: %s\n", status, checksum_char, calcul_integrity, characters);
        printf("ER : nombre d’erreur cumulée : %d\n", error_count);
    }

} /* key0_press_handler */



/***************************
 * Main entrypoint
 ***************************/

int main(void){

    TKeyPressAction key_press_actions[ARE_PW5_BUTTONS_NBR] = {
        {
            key0_press_handler,
            false,
			false
        },
        {
            key1_press_handler,
            false,
			false
        },
        {
			key2_press_handler,
            false,
			true
        }
    };

	/**
	 * Previous ON / OFF states of the switches
	 */
	uint32_t prev_switches_state;

	/**
	 * Current ON / OFF states of the switches
	 */
	uint32_t curr_switches_state;
    
    printf("Laboratoire: Conception d'une interface évoluée \n");
    printf("Design standard ID: %08X\n", (unsigned int)AXI_LW_CONST_REG);
    printf("Interface ID: %08X\n", (unsigned int)AXI_HPS_LABO_REG(ARE_PW5_INTERFACE_ADDR));
    
    /*
     * Init I/O
     */
    Keys_init();
    Switchs_init();
    Leds_init();

    /*
     * Main program
     */

    while(1)
    {
    	curr_switches_state = Switchs_read();

    	/* Update LEDs according to the enabled switches */
    	Leds_clear(ARE_PW5_LEDS_MASK);
    	Leds_set(curr_switches_state);

    	uint32_t switches_diff = curr_switches_state ^ prev_switches_state;
    	if(switches_diff)
    	{
        	/* Check if we need to change the generation frequency */
    		if(switches_diff & ARE_PW5_FREQ_SWITCHES_MASK)
    		{
    			are_pw5_gen_set_freq((curr_switches_state & ARE_PW5_FREQ_SWITCHES_MASK) >> ARE_PW5_FREQ_SWITCHES_OFFSET);
    		} /* if */

    		/* Does the mode need to change? */
    		if(switches_diff & ARE_PW5_MODE_SWITCHES_MASK)
    		{
                are_pw5_gen_set_mode((curr_switches_state & ARE_PW5_MODE_SWITCHES_MASK) >> ARE_PW5_MODE_SWITCHES_OFFSET);
    		} /* if */

    		/* What about the trustworthy reading operations? (Part II) */
    		if(switches_diff & ARE_PW5_TRUSTY_SWITCHES_MASK)
    		{
    			trustworthy_mode = (curr_switches_state & ARE_PW5_TRUSTY_SWITCHES_MASK) >> ARE_PW5_TRUSTY_SWITCHES_OFFSET;
    		} /* if */

    		prev_switches_state = curr_switches_state;
    	} /* if */



        for(size_t i = 0; i < ARE_PW5_BUTTONS_NBR; ++i)
        {
            if(Key_read(i))
            {
                if(!key_press_actions[i].allows_continuous_pressing
				 && key_press_actions[i].key_pressed)
                {
                    continue;
                } /* if */

                key_press_actions[i].on_press();
                key_press_actions[i].key_pressed = true;
            }
            else
            {
                key_press_actions[i].key_pressed = false;
            } /* if */
        } /* for */
    } /* while */

    return EXIT_SUCCESS;
}
