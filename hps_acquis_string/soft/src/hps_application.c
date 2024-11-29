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

int __auto_semihosting;


int main(void){
    
    printf("Laboratoire: Conception d'une interface évoluée \n");
    printf("Design standard ID: %08X", AXI_LW_CONST_REG);
    printf("Interface ID: %08X", AXI_HPS_LABO_REG(ARE_PW5_INTERFACE_ADDR));
    
    /*
     * Init I/O
     */
    Keys_init();
    Switchs_init();
    Leds_init();

    /*
     * Main program
     */

    return EXIT_SUCCESS;
}
