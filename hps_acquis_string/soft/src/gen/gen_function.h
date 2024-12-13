

#ifndef SRC_GEN_GEN_FUNCTION_H_
#define SRC_GEN_GEN_FUNCTION_H_

#include <stdint.h>
#include <stdbool.h>


/***************************
 * Triggering switches
 ***************************/

#define ARE_PW5_FREQ_SWITCHES_OFFSET	(8u)
#define ARE_PW5_FREQ_SWITCHES_MASK		((0b11) << ARE_PW5_FREQ_SWITCHES_OFFSET)

#define ARE_PW5_MODE_SWITCHES_OFFSET	(7u)
#define ARE_PW5_MODE_SWITCHES_MASK		((0b1) << ARE_PW5_MODE_SWITCHES_OFFSET)

#define ARE_PW5_TRUSTY_SWITCHES_OFFSET	(0u)	/* Part II */
#define ARE_PW5_TRUSTY_SWITCHES_MASK	(0b1)	/* Part II */


/***************************
 * Interface status (RO)
 ***************************/

#define ARE_PW5_STATUS_ADDR				(0x10)
#define ARE_PW5_STATUS_OFFSET			(0x00)
#define ARE_PW5_STATUS_MASK				(0x00000003 << ARE_PW5_STATUS_OFFSET)
#define ARE_PW5_STATUS_TRUSTY			(0b10 << ARE_PW5_STATUS_OFFSET)
#define ARE_PW5_SNAPSHOT_AVAIL_MASK		(0b01 << ARE_PW5_STATUS_OFFSET)


/***************************
 * Modes & delays (RW)
 ***************************/

#define ARE_PW5_DELAY_ADDR				(0x14)
#define ARE_PW5_DELAY_OFFSET			(0x00)
#define ARE_PW5_DELAY_MASK				(0x00000003 << ARE_PW5_DELAY_OFFSET)
#define ARE_PW5_DELAY_1HZ				(0b00 << ARE_PW5_DELAY_OFFSET)
#define ARE_PW5_DELAY_1KHZ				(0b01 << ARE_PW5_DELAY_OFFSET)
#define ARE_PW5_DELAY_100KHZ			(0b10 << ARE_PW5_DELAY_OFFSET)
#define ARE_PW5_DELAY_1MHZ				(0b11 << ARE_PW5_DELAY_OFFSET)

#define ARE_PW5_MODE_ADDR				ARE_PW5_DELAY_ADDR
#define ARE_PW5_MODE_OFFSET				(0x04)
#define ARE_PW5_MODE_MASK				(0x00000001 << ARE_PW5_MODE_OFFSET)
#define ARE_PW5_MODE_MANUAL				(0b0 << ARE_PW5_MODE_OFFSET)
#define ARE_PW5_MODE_AUTO				(0b1 << ARE_PW5_MODE_OFFSET)

#define ARE_PW5_TRUSTY_ADDR				(0x1C)
#define ARE_PW5_TRUSTY_OFFSET			(0x00)
#define ARE_PW5_TRUSTY_MASK				ARE_PW5_TRUSTY_SWITCHES_MASK


/***************************
 * Controls (WO)
 ***************************/

#define ARE_PW5_INIT_CHAR_ADDR			ARE_PW5_STATUS_ADDR
#define ARE_PW5_INIT_CHAR_MASK			(1u)
#define ARE_PW5_NEW_CHAR_ADDR			ARE_PW5_STATUS_ADDR
#define ARE_PW5_NEW_CHAR_MASK			(1u << 4u)
#define ARE_PW5_STABLE_READ_REQ_ADDR	(0x18)
#define ARE_PW5_STABLE_READ_REQ_MASK	(0b1)


/***************************
 * Characters (RO)
 ***************************/

#define ARE_PW5_CHAR_0_OFFSET			(0x00u)
#define ARE_PW5_CHAR_1_OFFSET			(0x01u)
#define ARE_PW5_CHAR_2_OFFSET			(0x02u)
#define ARE_PW5_CHAR_3_OFFSET			(0x03u)

#define ARE_PW5_CHARS_0_ADDR			(0x20)
#define ARE_PW5_CHARS_1_ADDR			(0x24)
#define ARE_PW5_CHARS_2_ADDR			(0x28)
#define ARE_PW5_CHARS_3_ADDR			(0x2C)

#define ARE_PW5_CHECKSUM_ADDR			(0x30)
#define ARE_PW5_CHECKSUM_OFFSET			(0x00)
#define ARE_PW5_CHECKSUM_MASK			(0xFF << ARE_PW5_CHECKSUM_OFFSET)

#define ARE_PW5_CHARS_BLOCKS			(4u)
#define ARE_PW5_CHARS_PER_ADDR			(4u)
#define ARE_PW5_CHARS_NUMBER			(16u)


/***************************
 * Functions
 ***************************/

/**
 * Sets a new frequency for the char generator
 * \param new_frequency New frequency. Not in terms of Hz, but in terms of the values that we're given
 */
void are_pw5_gen_set_freq
(
	uint32_t new_frequency
);

/**
 * Sets a new mode for the char generator
 * \param new_mode New mode
 */
void are_pw5_gen_set_mode
(
	uint32_t new_mode
);

/**
 * Reads the characters as sent by the board
 * \param enable When 1, enables that mode. Disables if 0
 */
void are_pw5_gen_read
(
	char  characters[ARE_PW5_CHARS_NUMBER],
	char* checksum
);

/**
 * Enables or disables the trustworthy reading mode (reliable reads)
 * \param enable When 1, enables the reliable reads. Disables if 0
 */
void are_pw5_gen_set_trusty
(
	char enabled
);


#endif /* SRC_GEN_GEN_FUNCTION_H_ */
