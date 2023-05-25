/* 
 * File:   chr_lcd.c
 * Author: Adnan
 *
 * Created on April 25, 2023, 11:45 PM
 */
#include "ecu_chr_lcd.h"
static std_ReturnType lcd_send_4bit(const chr_lcd_4bit_t *lcd_obj, uint8_t _data_command);
static std_ReturnType lcd_4bit_send_enable_signal(const chr_lcd_4bit_t *lcd_obj);
static std_ReturnType lcd_8bit_send_enable_signal(const chr_lcd_8bit_t *lcd_obj);
static std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd_obj, uint8_t row, uint8_t coloumn);
static std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd_obj, uint8_t row, uint8_t coloumn);


/**
 * 
 * @param lcd_obj
 * @return 
 */
std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t *lcd_obj)
{
    std_ReturnType ret_val= E_NOT_OK;
    uint8_t index_i= 0;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_initialize(&(lcd_obj->lcd_rs));
        ret_val= gpio_pin_initialize(&(lcd_obj->lcd_en));
        
        for(index_i=0; index_i < 4; index_i++)
        {
            ret_val= gpio_pin_initialize(&(lcd_obj->lcd_data[index_i]));
        }
        
        __delay_ms(20);
        ret_val= lcd_4bit_send_command(lcd_obj, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret_val= lcd_4bit_send_command(lcd_obj, _LCD_8BIT_MODE_2_LINE);
        __delay_us(120);
        ret_val= lcd_4bit_send_command(lcd_obj, _LCD_8BIT_MODE_2_LINE);
        
        ret_val= lcd_4bit_send_command(lcd_obj, _LCD_CLEAR);
        ret_val= lcd_4bit_send_command(lcd_obj, _LCD_RETURN_HOME);
        ret_val= lcd_4bit_send_command(lcd_obj, _LCD_ENTRY_MODE);
        ret_val= lcd_4bit_send_command(lcd_obj, _LCD_CURSOR_OFF_DISPLAY_ON);
        ret_val= lcd_4bit_send_command(lcd_obj, _LCD_4BIT_MODE_2_LINE);
        
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param command
 * @return 
 */
std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t *lcd_obj, uint8_t command)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_rs), GPIO_LOW);
        
        ret_val= lcd_send_4bit(lcd_obj, command >> 4);                   // Send 4 HIGH bits first
        
        // Sending Hight to Low transition on Enable Signal
        ret_val= lcd_4bit_send_enable_signal(lcd_obj);
        
        ret_val= lcd_send_4bit(lcd_obj, command);                       // Send 4 LOW bits
        
        // Sending Hight to Low transition on Enable Signal
        ret_val= lcd_4bit_send_enable_signal(lcd_obj);
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param data
 * @return 
 */
std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t *lcd_obj, uint8_t data)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_rs), GPIO_HIGH);
        
        ret_val= lcd_send_4bit(lcd_obj, data >> 4);                   // Send 4 HIGH bits first
        
        // Sending Hight to Low transition on Enable Signal
        ret_val= lcd_4bit_send_enable_signal(lcd_obj);
        
        ret_val= lcd_send_4bit(lcd_obj, data);                       // Send 4 LOW bits
        
        // Sending Hight to Low transition on Enable Signal
        ret_val= lcd_4bit_send_enable_signal(lcd_obj);
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param row
 * @param coloumn
 * @param data
 * @return 
 */
std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t *lcd_obj, uint8_t row, uint8_t coloumn, uint8_t data)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= lcd_4bit_set_cursor(lcd_obj, row, coloumn);
        ret_val= lcd_4bit_send_char_data(lcd_obj, data);
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param str
 * @return 
 */
std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t *lcd_obj, uint8_t *str)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if((NULL == lcd_obj) || (NULL == str))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_rs), GPIO_HIGH);
        
        while(*str)
        {
            ret_val= lcd_4bit_send_char_data(lcd_obj, *str);
            str++;                    
        }
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param row
 * @param coloumn
 * @param str
 * @return 
 */
std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t *lcd_obj, uint8_t row, uint8_t coloumn, uint8_t *str)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if((NULL == lcd_obj) || (NULL == str))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_rs), GPIO_HIGH);
        
        ret_val= lcd_4bit_set_cursor(lcd_obj, row, coloumn);
        while(*str)
        {
            ret_val= lcd_4bit_send_char_data(lcd_obj, *str);
            str++;                    
        }
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param row
 * @param coloumn
 * @param _chr
 * @param mem_pos
 * @return 
 */
std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t *lcd_obj, uint8_t row, uint8_t coloumn, 
                                                                const uint8_t _chr[], uint8_t mem_pos)
{
    std_ReturnType ret_val= E_NOT_OK;
    uint8_t index_i= 0;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= lcd_4bit_send_command(lcd_obj, (_LCD_CGRAM_START + (mem_pos * 8)));
        for(index_i= 0; index_i<8; index_i++)
        {
            ret_val= lcd_4bit_send_char_data(lcd_obj, _chr[index_i]);
        }
        ret_val= lcd_4bit_send_char_data_pos(lcd_obj, row, coloumn, mem_pos);
    }
    return ret_val;
}

// --------------------------------------------------------------------------------------------------------------------------------

/**
 * 
 * @param lcd_obj
 * @return 
 */
std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t *lcd_obj)
{
    std_ReturnType ret_val= E_NOT_OK;
    uint8_t index_i= 0;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_initialize(&(lcd_obj->lcd_rs));
        ret_val= gpio_pin_initialize(&(lcd_obj->lcd_en));
        
        for(index_i=0; index_i < 8; index_i++)
        {
            ret_val= gpio_pin_initialize(&(lcd_obj->lcd_data[index_i]));
        }
        __delay_ms(20);
        ret_val= lcd_8bit_send_command(lcd_obj, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret_val= lcd_8bit_send_command(lcd_obj, _LCD_8BIT_MODE_2_LINE);
        __delay_us(120);
        ret_val= lcd_8bit_send_command(lcd_obj, _LCD_8BIT_MODE_2_LINE);
        
        ret_val= lcd_8bit_send_command(lcd_obj, _LCD_CLEAR);
        ret_val= lcd_8bit_send_command(lcd_obj, _LCD_RETURN_HOME);
        ret_val= lcd_8bit_send_command(lcd_obj, _LCD_ENTRY_MODE);
        ret_val= lcd_8bit_send_command(lcd_obj, _LCD_CURSOR_OFF_DISPLAY_ON);
        
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param command
 * @return 
 */
std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t *lcd_obj, uint8_t command)
{
    std_ReturnType ret_val= E_NOT_OK;
    uint8_t index_i= 0;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_rs), GPIO_LOW);
        
        for(index_i= 0; index_i < 8; index_i++)
        {
            ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_data[index_i]), ((command >> index_i) & 0x01));
        }
        
        // Sending High to Low transition on Enable Signal
        ret_val= lcd_8bit_send_enable_signal(lcd_obj);
        
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param data
 * @return 
 */
std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t *lcd_obj, uint8_t data)
{
    std_ReturnType ret_val= E_NOT_OK;
    uint8_t index_i= 0;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_rs), GPIO_HIGH);
        
        for(index_i= 0; index_i < 8; index_i++)
        {
            ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_data[index_i]), ((data >> index_i) & 0x01));
        }
        
        // Sending Hight to Low transition on Enable Signal
        ret_val= lcd_8bit_send_enable_signal(lcd_obj);
        
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param row
 * @param coloumn
 * @param data
 * @return 
 */
std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t *lcd_obj, uint8_t row, uint8_t coloumn, uint8_t data)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= lcd_8bit_set_cursor(lcd_obj, row, coloumn);
        ret_val= lcd_8bit_send_char_data(lcd_obj, data);
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param str
 * @return 
 */
std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t *lcd_obj, uint8_t *str)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if((NULL == lcd_obj) || (NULL == str))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_rs), GPIO_HIGH);
        
        while(*str)
        {
            ret_val= lcd_8bit_send_char_data(lcd_obj, *str);
            str++;                    
        }
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param row
 * @param coloumn
 * @param str
 * @return 
 */
std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t *lcd_obj, uint8_t row, uint8_t coloumn, uint8_t *str)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if((NULL == lcd_obj) || (NULL == str))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= lcd_8bit_set_cursor(lcd_obj, row, coloumn);
        while(*str)
        {
            ret_val= lcd_8bit_send_char_data(lcd_obj, *str);
            str++;                    
        }
    }
    return ret_val;
}

/**
 * 
 * @param lcd_obj
 * @param row
 * @param coloumn
 * @param _chr
 * @param mem_pos
 * @return 
 */
std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t *lcd_obj, uint8_t row, uint8_t coloumn, 
                                                                const uint8_t _chr[], uint8_t mem_pos)
{
    std_ReturnType ret_val= E_NOT_OK;
    uint8_t index_i= 0;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= lcd_8bit_send_command(lcd_obj, (_LCD_CGRAM_START + (mem_pos * 8)));
        for(index_i= 0; index_i<8; index_i++)
        {
            ret_val= lcd_8bit_send_char_data(lcd_obj, _chr[index_i]);
        }
        ret_val= lcd_8bit_send_char_data_pos(lcd_obj, row, coloumn, mem_pos);
    }
    return ret_val;
}

// -------------------------------------------------------------------------------------------------------------------------------

/**
 * 
 * @param value
 * @param str
 */
std_ReturnType convert_byte_to_string(uint8_t value, uint8_t *str)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == str)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        memset(str, '\0', 4);
        sprintf(str, "%i", value);
    }
    return ret_val;
}

/**
 * 
 * @param value
 * @param str
 */
std_ReturnType convert_short_to_string(uint16_t value, uint8_t *str)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == str)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        memset(str, '\0', 6);
        sprintf(str, "%i", value);
    }
    return ret_val;
}

/**
 * 
 * @param value
 * @param str
 */
std_ReturnType convert_int_to_string(uint32_t value, uint8_t *str)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == str)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        memset(str, '\0', 11);
        sprintf(str, "%i", value);
    }
    return ret_val;
}


/* ---------------------------------- HELPER FUNCTIONS ----------------------------------*/
static std_ReturnType lcd_send_4bit(const chr_lcd_4bit_t *lcd_obj, uint8_t _data_command)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_data[0]), ((_data_command >> 0) & 0x01));
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_data[1]), ((_data_command >> 1) & 0x01));
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_data[2]), ((_data_command >> 2) & 0x01));
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_data[3]), ((_data_command >> 3) & 0x01));
    
    }
    return ret_val;
}

static std_ReturnType lcd_4bit_send_enable_signal(const chr_lcd_4bit_t *lcd_obj)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_en), GPIO_HIGH);
        __delay_us(5);
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_en), GPIO_LOW);
        
    }
    return ret_val;
}

static std_ReturnType lcd_8bit_send_enable_signal(const chr_lcd_8bit_t *lcd_obj)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_en), GPIO_HIGH);
        __delay_us(5);
        ret_val= gpio_pin_write_logic(&(lcd_obj->lcd_en), GPIO_LOW);
        
    }
    return ret_val;
}

static std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd_obj, uint8_t row, uint8_t coloumn)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        coloumn--;
        switch(row)
        {
            case ROW1:
                ret_val= lcd_8bit_send_command(lcd_obj, (0x80 + coloumn));
                break;
                
            case ROW2:
                ret_val= lcd_8bit_send_command(lcd_obj, (0xc0 + coloumn));
                break;
                
            case ROW3:
                ret_val= lcd_8bit_send_command(lcd_obj, (0x94 + coloumn));
                break;
                
            case ROW4:
                ret_val= lcd_8bit_send_command(lcd_obj, (0xd4 + coloumn));
                break;
        }
    }
    return ret_val;
}

static std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd_obj, uint8_t row, uint8_t coloumn)
{
    std_ReturnType ret_val= E_NOT_OK;
    
    if(NULL == lcd_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        coloumn--;
        switch(row)
        {
            case ROW1:
                ret_val= lcd_4bit_send_command(lcd_obj, (0x80 + coloumn));
                break;
                
            case ROW2:
                ret_val= lcd_4bit_send_command(lcd_obj, (0xc0 + coloumn));
                break;
                
            case ROW3:
                ret_val= lcd_4bit_send_command(lcd_obj, (0x94 + coloumn));
                break;
                
            case ROW4:
                ret_val= lcd_4bit_send_command(lcd_obj, (0xd4 + coloumn));
                break;
        }
    }
    return ret_val;
}