/* 
 * File:   application.c
 * Author: Adnan
 *
 * Created on February 4, 2023, 1:59 PM
 */

#include "application.h"

void I2C_Send_1Byte(uint8_t slave_add, uint8_t _data);

void MSSP_I2C_DfeaultInterruptHandler(void);

std_ReturnType ret_val= E_NOT_OK;
uint8_t slave_ack;
volatile uint8_t i2c_slave1_var;

#define SLAVE_1     0x60
#define SLAVE_2     0x61

i2c_t i2c_obj={
    .i2c_cfg.i2c_mode= I2C_SLAVE_MODE,
    .i2c_cfg.i2c_mode_cfg= I2C_SLAVE_MODE_7BIT_ADDRESS_CFG,
    .i2c_cfg.i2c_slave_address= SLAVE_2,
    .i2c_cfg.i2c_SMBUS_control= I2C_SMBUS_DISABLE_CFG,
    .i2c_cfg.i2c_slew_rate= I2C_SLEW_RATE_DISABLE_CFG,
    .i2c_cfg.i2c_general_call= I2C_GENERAL_CALL_DISABLE_CFG,
    .I2C_InterruptHandler= MSSP_I2C_DfeaultInterruptHandler,
    .I2C_Report_Receive_Overflow= NULL,
    .I2C_Report_Write_Collision= NULL
};


int main() {
    application_initialize();
    
    ret_val= I2C_Init(&i2c_obj);
    
    while(1)
    {
        
    }
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ecu_layer_initialize();
}   

void I2C_Send_1Byte(uint8_t slave_add, uint8_t _data)
{
    ret_val= I2C_Master_Send_Start(&i2c_obj);
    ret_val= I2C_Write(&i2c_obj, slave_add, &slave_ack);
    ret_val= I2C_Write(&i2c_obj, _data, &slave_ack);
    ret_val= I2C_Master_Send_Stop(&i2c_obj);
}

void MSSP_I2C_DfeaultInterruptHandler(void)
{
    /* Stretch Clock (Hold Clock LOW) */
    I2C_CLOCK_STRETCH_ENABLE();
    
    if((SSPSTATbits.R_nW == 0) && (SSPSTATbits.D_nA == 0)) /* Master will Write Data */
    {
        uint8_t dummy_buffer= SSPBUF;       /* Read the Least Byte To Clear the Buffer */
        while(!SSPSTATbits.BF);
        i2c_slave1_var= SSPBUF;             /* Read Data */
    }
    else if(SSPSTATbits.R_nW == 1)  /* Master needs to read Data */
    {
        
    }
    /* Release Clock */
    I2C_CLOCK_STRETCH_DISABLE();
}