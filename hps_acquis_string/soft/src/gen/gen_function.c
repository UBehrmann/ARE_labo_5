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

#include "gen_function.h"
#include "../axi_lw.h"


void are_pw5_gen_set_freq
(
	uint32_t new_frequency
)
{
	AXI_HPS_LABO_REG(ARE_PW5_DELAY_ADDR) = (new_frequency << ARE_PW5_DELAY_OFFSET) & ARE_PW5_DELAY_MASK;
} /* are_pw5_gen_set_freq */


void are_pw5_gen_set_mode
(
	uint32_t new_mode
)
{
	AXI_HPS_LABO_REG(ARE_PW5_MODE_ADDR) = (new_mode << ARE_PW5_MODE_OFFSET) & ARE_PW5_MODE_MASK;
} /* are_pw5_gen_set_mode */

void are_pw5_gen_set_freq_and_mode
(
	uint32_t new_frequency,
	uint32_t new_mode
)
{
	AXI_HPS_LABO_REG(ARE_PW5_DELAY_ADDR) = (new_frequency << ARE_PW5_DELAY_OFFSET) & ARE_PW5_DELAY_MASK;
	AXI_HPS_LABO_REG(ARE_PW5_MODE_ADDR) |= (new_mode << ARE_PW5_MODE_OFFSET) & ARE_PW5_MODE_MASK;
} /* are_pw5_gen_set_freq_and_mode */


void are_pw5_gen_enable_trusty
(
	uint32_t enable
)
{

} /* are_pw5_gen_enable_trusty */
