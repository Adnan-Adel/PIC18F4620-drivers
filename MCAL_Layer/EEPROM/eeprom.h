/* 
 * File:   ecu_eeprom.h
 * Author: Adnan
 *
 * Created on May 31, 2023, 1:28 PM
 */

#ifndef ECU_EEPROM_H
#define	ECU_EEPROM_H

/* Section: Includes */
#include "../mcal_std_types.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "pic18f4620.h"

/* Section: Macro Declarations */

/* Flash Programm or Data EEPROM select */
#define ACCESS_FLASH_PROGRAMM_MEMORY                1
#define ACCEESS_EEPROM_DATA_MEMORY                  0

/* Flash Programm/Data EEPROM or configuration select */
#define ACCESS_CONFIG_REGISTERS                     1
#define ACCEESS_FLASH_EEPROM_MEMORY                 0

/* Flash Programm/Data EEPROM Write Enable */
#define ALLOW_WRITE_CYCLES_FLASH_EEPROM             1
#define INHIBITS_WRITE_CYCLES_FLASH_EEPROM          0

/* Write Control */
#define INITIATE_DATA_EEPROM_WRITE_ERASE            1
#define DATA_EEPROM_WRITE_ERASE_COMPLETE            0

/* Read Control */
#define INITIATE_DATA_EEPROM_READ                   1

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */


/* Section: Software Interface Declarations */
std_ReturnType Data_EEPROM_WriteByte(uint16_t bAdd, uint8_t bData);
std_ReturnType Data_EEPROM_ReadByte(uint16_t bAdd, uint8_t *bData);

#endif	/* ECU_EEPROM_H */

