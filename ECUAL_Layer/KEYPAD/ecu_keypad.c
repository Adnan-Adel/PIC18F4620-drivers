/* 
 * File:   ecu_keypad.h
 * Author: Adnan
 *
 * Created on March 2, 2023, 4:48 PM
 */
#include "ecu_keypad.h"

static const uint8_t btn_values[KEYPAD_ROWS][KEYPAD_COLUMNS]={
    {'7','8','9','/'},
    {'4','5','6','*'},
    {'1','2','3','-'},
    {'#','0','=','+'}
};

/**
 * 
 * @param keypad_obj: pointer to the keypad configurations
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType keypad_initialize(const keypad_t *keypad_obj)
{
    std_ReturnType ret_val= E_NOT_OK;
    uint8_t rows_counter=0, columns_counter=0;
    if(NULL == keypad_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        for(rows_counter=0; rows_counter<KEYPAD_ROWS; rows_counter++)
        {
            ret_val= gpio_pin_initialize(&(keypad_obj->keypad_rows_pins[rows_counter]));
        }
        for(columns_counter=0; columns_counter<KEYPAD_ROWS; columns_counter++)
        {
            ret_val= gpio_pin_direction_initialize(&(keypad_obj->keypad_rows_pins[columns_counter]));
        }
    }
    return ret_val;
}

/**
 * 
 * @param keypad_obj: pointer to the keypad configurations
 * @param value: value of button pressed by the user
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType keypad_get_value(const keypad_t *keypad_obj, uint8_t *value)
{
    std_ReturnType ret_val= E_NOT_OK;
    uint8_t l_rows_counter=0, l_columns_counter=0, l_counter=0;
    uint8_t column_logic=0;
    if((NULL == keypad_obj) || (NULL == value))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        for(l_rows_counter=0; l_rows_counter<KEYPAD_ROWS; l_rows_counter++)
        {
            for(l_counter=0; l_counter<KEYPAD_ROWS; l_counter++)
            {
                ret_val= gpio_pin_write_logic(&(keypad_obj->keypad_rows_pins[l_counter]), GPIO_LOW);
            }
                ret_val= gpio_pin_write_logic(&(keypad_obj->keypad_rows_pins[l_rows_counter]), GPIO_HIGH);
                __delay_ms(10);
            
            for(l_columns_counter=0; l_columns_counter<KEYPAD_COLUMNS; l_columns_counter++)
            {
                ret_val= gpio_pin_read_logic(&(keypad_obj->keypad_columns_pins[l_columns_counter]), &column_logic);
                if(GPIO_HIGH == column_logic)
                {
                    *value= btn_values[l_rows_counter][l_columns_counter];
                }
            }
        }
    }
    return ret_val;
}
