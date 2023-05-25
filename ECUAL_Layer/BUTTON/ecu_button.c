/* 
 * File:   ecu_button.c
 * Author: Adnan
 *
 * Created on February 11, 2023, 1:21 PM
 */
#include "ecu_button.h"

/**
 * @brief Initialize the assigned pin to be INPUT
 * @param btn: pointer to button configurations
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType button_initialize(const button_t *btn)
{
    std_ReturnType ret_val= E_OK;
    
    if(NULL == btn)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_direction_initialize(&(btn->button_pin));
    }
    
    return ret_val;
}

/**
 * @brief Read the state of the button
 * @param btn: pointer to button configurations
 * @param btn_state
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType button_read_state(const button_t *btn, button_state_t *btn_state)
{
    std_ReturnType ret_val= E_OK;
    logic_t pin_state= GPIO_LOW;
    
    if((NULL == btn) || (NULL == btn_state))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        gpio_pin_read_logic(&(btn->button_pin), &pin_state);
        
        if(BUTTON_ACTIVE_HIGH == btn->button_connection_state)
        {
            if(GPIO_HIGH == pin_state)
            {
                *btn_state= BUTTON_PRESSED;
            }
            else
            {
                *btn_state= BUTTON_RELEASED;
            }
        }
        else if(BUTTON_ACTIVE_LOW ==  btn->button_connection_state)
        {
            if(GPIO_HIGH == pin_state)
            {
                *btn_state= BUTTON_RELEASED;
            }
            else
            {
                *btn_state= BUTTON_PRESSED;
            }
        }
    }
    return ret_val;
}
