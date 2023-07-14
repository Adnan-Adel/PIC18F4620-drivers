/* 
 * File:   ecu_eeprom.c
 * Author: Adnan
 *
 * Created on May 31, 2023, 1:28 PM
 */
#include "eeprom.h"
/**
 * 
 * @param bAdd
 * @param bData
 * @return 
 */
std_ReturnType Data_EEPROM_WriteByte(uint16_t bAdd, uint8_t bData)
{
    std_ReturnType ret_val= E_OK;
    /* Read Interrupt Status (Enabled or Disabled) */
    uint8_t Global_Interrupt_Status= INTCONbits.GIE;
    /* Update Address Register */
    EEADRH= (uint8_t)((bAdd >> 8) & 0x03);
    EEADR= (uint8_t)(bAdd & 0xFF);
    /* Update Data Register */
    EEDATA= bData;
    /* Select Access data EEPROM memory */
    EECON1bits.EEPGD= ACCEESS_EEPROM_DATA_MEMORY;
    EECON1bits.CFGS= ACCEESS_FLASH_EEPROM_MEMORY;
    /* Allow write cycles to Flash Program/Data EEPROM */
    EECON1bits.WREN= ALLOW_WRITE_CYCLES_FLASH_EEPROM;
    /* Disable all Interrupts "General Interrupts" */
    INTERRUPT_GlobalInterruptDisable();
    /* Write the required sequence : 0x55 -> 0xAA */
    EECON2= 0x55;
    EECON2= 0xAA;
    /* Initiates a data EEPROM erase/write cycle */
    EECON1bits.WR= INITIATE_DATA_EEPROM_WRITE_ERASE;
    /* Wait for Write to complete */
    while(EECON1bits.WR);
    /* Inhibits write cycles to Flash Program/Data EEPROM */
    EECON1bits.WREN= INHIBITS_WRITE_CYCLES_FLASH_EEPROM;
    /* Restore the Global Interrupt status */
    INTCONbits.GIE= Global_Interrupt_Status;
    
    return ret_val;
}

/**
 * 
 * @param bAdd
 * @param bData
 * @return 
 */
std_ReturnType Data_EEPROM_ReadByte(uint16_t bAdd, uint8_t *bData)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == bData)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Update Address Register */
        EEADRH= (uint8_t)((bAdd >> 8) & 0x03);
        EEADR= (uint8_t)(bAdd & 0xFF);
        /* Select Access data EEPROM memory */
        EECON1bits.EEPGD= ACCEESS_EEPROM_DATA_MEMORY;
        EECON1bits.CFGS= ACCEESS_FLASH_EEPROM_MEMORY;
        /* Initiate a data EEPROM Read Cycle */
        EECON1bits.RD= INITIATE_DATA_EEPROM_READ;
        /* For High Frequency */
        NOP();
        NOP();
        /* Return Data */
        *bData= EEDATA;
    }
    
    return ret_val;
}