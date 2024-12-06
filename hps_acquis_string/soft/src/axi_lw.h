/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : axi_lw.h
 * Author               : Anthony Convers
 * Date                 : 27.07.2022
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Header file for bus AXI lightweight HPS to FPGA defines definition
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 0.0    27.07.2022  ACS           Initial version.
 *
*****************************************************************************************/
#include <stdint.h>

// Base address
#define AXI_LW_HPS_FPGA_BASE_ADD   0xFF200000

// ACCESS MACROS
#define AXI_LW_REG(_x_)   *(volatile uint32_t *)(AXI_LW_HPS_FPGA_BASE_ADD + _x_) // _x_ is an offset with respect to the base address

/**
 * Address of the 32bit constant
 */
#define AXI_LW_CONST_REG			(AXI_LW_REG(0))

/* Base address for this PW's interface memory space */
#define AXI_LW_HPS_FPGA_LABO_ADD	0x010000

/* Gets the register address for this PW's memory space, with an offset */
#define AXI_HPS_LABO_REG(offset)	(AXI_LW_REG(AXI_LW_HPS_FPGA_LABO_ADD + offset))

/* Interface ID (RO) */
#define ARE_PW5_INTERFACE_ADDR		(0x00)


