/* 
 * File:   ecu_relay.c
 * Author: Adnan
 *
 * Created on February 12, 2023, 6:12 PM
 */
#include "ecu_relay.h"

/**
 * @brief Initialize the assigned pin to be OUTPUT and turn the relay off
 * @param relay: pointer to the relay module configurations 
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType relay_initialize(const relay_t *relay)
{
    std_ReturnType ret_val= E_NOT_OK;
    if(NULL == relay)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        pin_config_t pin_obj= {.port= relay->relay_port, .pin= relay->relay_pin,
                                 .direction= GPIO_DIRECTION_OUTPUT, .logic= relay->relay_status};
        gpio_pin_initialize(&pin_obj);
    }
    return ret_val;
}

/**
 * @brief Turn the relay ON
 * @param relay: pointer to the relay module configurations 
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function 
 */
std_ReturnType relay_turn_on(const relay_t *relay)
{
    std_ReturnType ret_val= E_NOT_OK;
    if(NULL == relay)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        pin_config_t pin_obj= {.port= relay->relay_port, .pin= relay->relay_pin,
                                 .direction= GPIO_DIRECTION_OUTPUT, .logic= relay->relay_status};
        gpio_pin_write_logic(&pin_obj, GPIO_HIGH);
    }
    return ret_val;
}

/**
 * @brief Turn the relay OFF
 * @param relay: pointer to the relay module configurations 
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function 
 */
std_ReturnType relay_turn_off(const relay_t *relay)
{
    std_ReturnType ret_val= E_NOT_OK;
    if(NULL == relay)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        pin_config_t pin_obj= {.port= relay->relay_port, .pin= relay->relay_pin,
                                 .direction= GPIO_DIRECTION_OUTPUT, .logic= relay->relay_status};
        gpio_pin_write_logic(&pin_obj, GPIO_LOW);
    }
    return ret_val;
}

