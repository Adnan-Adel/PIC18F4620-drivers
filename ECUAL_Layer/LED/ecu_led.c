/* 
 * File:   ecu_led.h
 * Author: Adnan
 *
 * Created on February 4, 2023, 2:17 PM
 */
#include "ecu_led.h"

/**
 * @brief Initialize the assigned pin to be OUTPUT and turn the led OFF
 * @param led : Pointer to the led module configurations
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType led_initialize(const led_t *led)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == led)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        pin_config_t pin_obj={.port= led->port,                      .pin= led->pin,
                              .direction=GPIO_DIRECTION_OUTPUT,      .logic= led->led_status };
        
        gpio_pin_initialize(&pin_obj);
    }
    return ret_val;
}

/**
 * @brief Turn the led ON
 * @param led : Pointer to the led module configurations
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType led_turn_on(const led_t *led)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == led)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        pin_config_t pin_obj={.port= led->port,                      .pin= led->pin,
                              .direction=GPIO_DIRECTION_OUTPUT,      .logic= led->led_status };
        
        gpio_pin_write_logic(&pin_obj, GPIO_HIGH);
    }
    return ret_val;
}

/**
 * @brief Turn the led OFF
 * @param led : Pointer to the led module configurations
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType led_turn_off(const led_t *led)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == led)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        pin_config_t pin_obj={.port= led->port,                      .pin= led->pin,
                              .direction=GPIO_DIRECTION_OUTPUT,      .logic= led->led_status };
        
        gpio_pin_write_logic(&pin_obj, GPIO_LOW);
    }
    return ret_val;
}

/**
 * @brief Toggle the led
 * @param led : Pointer to the led module configurations
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType led_toggle(const led_t *led)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == led)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        pin_config_t pin_obj={.port= led->port,                      .pin= led->pin,
                              .direction=GPIO_DIRECTION_OUTPUT,      .logic= led->led_status };
        
        gpio_pin_toggle_logic(&pin_obj);
    }
    return ret_val;
}