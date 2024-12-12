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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gen_function.h"
#include "../axi_lw.h"


void are_pw5_gen_set_freq
(
	uint32_t new_frequency
)
{
	int prev_value = AXI_HPS_LABO_REG(ARE_PW5_DELAY_ADDR) & ~ARE_PW5_DELAY_MASK;
	prev_value |= (new_frequency << ARE_PW5_DELAY_OFFSET) & ARE_PW5_DELAY_MASK;
	AXI_HPS_LABO_REG(ARE_PW5_DELAY_ADDR) = prev_value;
} /* are_pw5_gen_set_freq */


void are_pw5_gen_set_mode
(
	uint32_t new_mode
)
{
	/* Erase bits that about to be written */
	int prev_value = AXI_HPS_LABO_REG(ARE_PW5_MODE_ADDR) & ~ARE_PW5_MODE_MASK;
	prev_value |= (new_mode << ARE_PW5_MODE_OFFSET) & ARE_PW5_MODE_MASK;
	AXI_HPS_LABO_REG(ARE_PW5_MODE_ADDR) = prev_value;
} /* are_pw5_gen_set_mode */


void are_pw5_gen_read
(
	char  characters[ARE_PW5_CHARS_NUMBER],
	char* checksum
)
{
	/* Board is in a trusty mode (reads are guaranteed to be correct) */
	//printf("[DEBUG] Status: %X\n[DEBUG] Snapshot request reg: %X\n",
	//	   (unsigned int)AXI_HPS_LABO_REG(ARE_PW5_STATUS_ADDR),
	//	   (unsigned int)AXI_HPS_LABO_REG(ARE_PW5_STABLE_READ_REQ_ADDR));

	if(ARE_PW5_STATUS_TRUSTY == (AXI_HPS_LABO_REG(ARE_PW5_STATUS_ADDR) & 0x02))
	{
		//printf("[DEBUG] Trustworthy read mode\n");
		/* Request snapshot, and wait until it's available */
		AXI_HPS_LABO_REG(ARE_PW5_STABLE_READ_REQ_ADDR) = ARE_PW5_STABLE_READ_REQ_MASK;
		while(ARE_PW5_SNAPSHOT_AVAIL_MASK != (AXI_HPS_LABO_REG(ARE_PW5_STATUS_ADDR) & ARE_PW5_SNAPSHOT_AVAIL_MASK))
		{
			//printf("[DEBUG] Waiting for snapshot\n");
			/* Wait */
		}
	} /* if */

	/* Read characters */
	const size_t base_char_offset = (ARE_PW5_CHARS_PER_ADDR - 1) * 8;
	for(size_t i = 0; i < ARE_PW5_CHARS_BLOCKS ; ++i)
	{
		uint32_t char_block = AXI_HPS_LABO_REG(ARE_PW5_CHARS_0_ADDR + i * sizeof(uint32_t));
		for(size_t j = 0; j < ARE_PW5_CHARS_PER_ADDR ; ++j)
		{
			characters[i * ARE_PW5_CHARS_PER_ADDR + j] = char_block >> (base_char_offset - j * 8) & 0xFF;
		} /* for */
	} /* for */

	*checksum = AXI_HPS_LABO_REG(ARE_PW5_CHECKSUM_ADDR);
} /* are_pw5_gen_read */
